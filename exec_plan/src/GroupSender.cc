#include <cassert>
#include <stdlib.h>
#include <algorithm>
#include <boost/functional/hash.hpp>

#include "GroupSender.h"
#include "BlockSerializer.h"
#include "layers.h"

namespace Engine {

struct HashFunction {
	int row;

	HashFunction(int row) : row(row) {}

	// hash function copied from
	// http://www.concentric.net/~Ttwang/tech/inthash.htm
	static inline unsigned int hashUnsigned( unsigned int a) {
		a = (a+0x7ed55d16) + (a<<12);
		a = (a^0xc761c23c) ^ (a>>19);
		a = (a+0x165667b1) + (a<<5);
		a = (a+0xd3a2646c) ^ (a<<9);
		a = (a+0xfd7046c5) + (a<<3);
		a = (a^0xb55a4f09) ^ (a>>16);
		return a;
	}

	inline long operator() (void *column, OperationTree::ScanOperation_Type type) {
		switch (type) {
	
			case OperationTree::ScanOperation_Type_INT:
				return hashUnsigned( *((int*) column + row) ); 

			case OperationTree::ScanOperation_Type_DOUBLE:
				assert(false && "there should be no hashing of doubles!");
				break;

			default:
				return *((bool*) column + row); // identity hash function for bools
		}
	}
};



GroupSender::GroupSender(NodeEnvironmentInterface *nei, Operation *source, std::vector<OperationTree::ScanOperation_Type> source_types,
		std::vector<OperationTree::ScanOperation_Type> hash_column_types,  const OperationTree::GroupByOperation &node
) : 
	Operation(nei), nei_(nei), source_(source), source_types_(source_types), hash_column_types_(hash_column_types), node_(node) {

	}

// dataToHash are columns that we should calculate hash for (not all the columns)
int32* GroupSender::count_hashes(const vector<void*> &dataToHash, 
		const vector<OperationTree::ScanOperation_Type> &typesToHash, int rows) {
	
	// temporary vector to count hashes for each column
	vector<int32> columnHashValues = vector<int32>(dataToHash.size());
	
	// array with hashes for the whole rows
	int32* res = new int32[rows];

	// calculate hash for each row independently
	for (int row = 0; row < rows; ++row) {

		// calculate hash function for each column first
		HashFunction hashFunction(row);
		transform(dataToHash.begin(), dataToHash.end(), typesToHash.begin(), columnHashValues.begin(), hashFunction);

		// combine hash values to obtain one hash value for the whole row
		res[row] = boost::hash_range(columnHashValues.begin(), columnHashValues.end());
	}	

	return res;
}

void GroupSender::scatter_data_into_buckets(vector<void*> data, int rows, int32* hashes) {
	for (int row = 0; row < rows; ++row) {

		// calculate the number of bucket
		int bucket = hashes[row] % Layers::count_nodes_in_other_layer();

		// copy the data to the bucket
		for (int column = 0, columns = data.size(); column < columns; ++columns)
			switch (source_types_[columns]) {
				case OperationTree::ScanOperation_Type_INT:
					std::fill_n( (int*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((int*) data[columns] + row));
					break;
				case OperationTree::ScanOperation_Type_DOUBLE:
					std::fill_n( (double*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((double*) data[columns] + row));
				case OperationTree::ScanOperation_Type_BOOL:
					std::fill_n( (bool*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((bool*) data[columns] + row));
			}

		buckets_load_[bucket]++;
	}
}

// take all the columns and returns columns for which hashes will be computed 
// (columns we will group by)
void GroupSender::cast_to_hash_columns(const vector<void*> &data, vector<void*> &result) {
	// TODO change this dummy implementation
	result = data;
}

bool GroupSender::bucket_ready_to_send(int bucket) {
	// only example implementation
	return buckets_load_[bucket] > 1000;
}

void GroupSender::send_bucket(int bucket_number){
				// serialize data
				char* serializedData;
				BlockSerializer serializer;
				int message_len = serializer.serializeBlock(source_types_, buckets_[bucket_number], buckets_load_[bucket_number], &serializedData);

				int who = Layers::get_real_node_number(1 - Layers::get_my_layer(), bucket_number);
				// send datas
				nei_ -> SendPacket(who, serializedData, message_len);

				// reset this bucket
				buckets_load_[bucket_number] = 0;
}

vector<void*> GroupSender::pull(int &rows) {

	sleep(4);
	
	int nrows = 0;
	vector<void*> data;

	do {
		data = source_->pull(rows);

		// cast columns to columns for which we computw hashes
		vector<void*> hashed_columns_data;
		cast_to_hash_columns(data, hashed_columns_data);
	
		int32* hashes = count_hashes(hashed_columns_data, hash_column_types_, rows);

		scatter_data_into_buckets(data, rows, hashes);

		for(int i = 0, buckets_no = buckets_.size(); i < buckets_no; ++i)
			if (bucket_ready_to_send(i)) {
				send_bucket(i);
			}

	} while (rows > 0);
	
	//all data pulled, sending remaining buckets and then always 0-sized packet as EOF
	for(int i = 0, buckets_no = buckets_.size(); i < buckets_no; ++i){
		if (buckets_load_[i] > 0) {
			send_bucket(i);
		}
		send_bucket(i);
	}
	
	// this is dummy return value
	return std::vector<void*>();
}

}
