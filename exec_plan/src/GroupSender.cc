#include <cassert>
#include <stdlib.h>
#include <algorithm>
#include <boost/functional/hash.hpp>

#include "GroupSender.h"
#include "BlockSerializer.h"
#include "layers.h"
#include "MEngine.h"

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


static const int bucket_size = 2 * BUFF_SIZE + 20;
static const int bucket_load_to_send = BUFF_SIZE;

GroupSender::GroupSender(NodeEnvironmentInterface *nei, Operation *source, std::vector<OperationTree::ScanOperation_Type> source_types,
		std::vector<OperationTree::ScanOperation_Type> hash_column_types,  const OperationTree::GroupByOperation &node
) : 
	Operation(nei), nei_(nei), source_(source), source_types_(source_types), hash_column_types_(hash_column_types), node_(node) {
		buckets_ = std::vector<std::vector<void*> >(Layers::count_nodes_in_other_layer());
		buckets_load_ = std::vector<int>(Layers::count_nodes_in_other_layer());
		for (int i = 0; i < Layers::count_nodes_in_other_layer();	++i) {
			buckets_[i] = std::vector<void*>(source_types.size());
			for (int j = 0; j <source_types.size(); ++j)
				buckets_[i][j] = malloc(bucket_size * sizeof(double));
		}
		
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
		int p = Layers::count_nodes_in_other_layer();
		int bucket = ((hashes[row] % p) + p) % p;
		cerr << "BUCKET " << bucket << "\n";
		assert(bucket < buckets_.size());

		// copy the data to the bucket
		for (int column = 0, columns = data.size(); column < columns; ++column)
			switch (source_types_[column]) {
				case OperationTree::ScanOperation_Type_INT:
					std::fill_n( (int32*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((int32*) data[column] + row));
					break;
				case OperationTree::ScanOperation_Type_DOUBLE:
					std::fill_n( (double*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((double*) data[column] + row));
					break;
				case OperationTree::ScanOperation_Type_BOOL:
					std::fill_n( (bool*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((bool*) data[column] + row));
					break;
			}

		buckets_load_[bucket]++;
	}
}

void GroupSender::cast_to_hash_columns(const vector<void*> &data, vector<void*> &result) {
	for(int i = 0; i < node_.group_by_column_size(); ++i) {
		result.push_back(data[node_.group_by_column(i)]);
	}
}

bool GroupSender::bucket_ready_to_send(int bucket) {
	// only example implementation
	return buckets_load_[bucket] >= bucket_load_to_send;
}

void GroupSender::send_bucket(int bucket_number){
				// serialize data
				char* serializedData;
				BlockSerializer serializer;
				cerr << "BEFORE SERIALIZATION: \n";
				printCols(source_types_, buckets_[bucket_number], buckets_load_[bucket_number]);

				int rows_to_send = min(buckets_load_[bucket_number], BUFF_SIZE);

				std::vector<void*> columns_with_offset = buckets_[bucket_number];
				for (int i = 0, columns = columns_with_offset.size(); i < columns; ++i)
					switch (source_types_[i]) {
						case OperationTree::ScanOperation_Type_INT:
							columns_with_offset[i] = ((int32*) columns_with_offset[i]) + buckets_load_[bucket_number] - rows_to_send;
							break;
						case OperationTree::ScanOperation_Type_BOOL:
							columns_with_offset[i] = ((bool*) columns_with_offset[i]) +buckets_load_[bucket_number] - rows_to_send;
							break;
						case OperationTree::ScanOperation_Type_DOUBLE:
							columns_with_offset[i] = ((double*) columns_with_offset[i]) +buckets_load_[bucket_number] - rows_to_send;
							break;
					}
				int message_len = serializer.serializeBlock(source_types_, columns_with_offset, rows_to_send, &serializedData);
				//int message_len = serializer.serializeBlock(source_types_, buckets_[bucket_number], buckets_load_[bucket_number], &serializedData);
				// ...
				cerr << "AFTER DESRIALIZATION: \n";
				vector<void*> bufs;
				int rows = serializer.deserializeBlock(source_types_, message_len, serializedData, bufs);
				printCols(source_types_, bufs, rows);
				cerr << "END\n";

				// ...

				int who = Layers::get_real_node_number(1 - Layers::get_my_layer(), bucket_number);
				// send datas
				cerr << "Send bucket to "<<who << endl;
				nei_ -> SendPacket(who, serializedData, message_len);

				// reset this bucket
				buckets_load_[bucket_number] -= rows_to_send;
				//buckets_load_[bucket_number] = 0;
}

vector<void*> GroupSender::pull(int &rows) {
	cerr << "GroupSender::pull " <<rows << endl;
	int nrows = 0;
	vector<void*> data;

	do {
		data = source_->pull(rows);

		cerr << "SCOLS: " << source_types_.size() << "COLS: " <<data.size() <<" ROWS: "<< rows << "\n";
		for(int row = 0; row < rows; row++){
			for (int column = 0, columns = data.size(); column < columns; ++column) {
				cerr << "ROW: ";
				switch (source_types_[column]) {
					case OperationTree::ScanOperation_Type_INT:
						cerr << * ((int32*) data[column] + row) <<" ";
						break;
					case OperationTree::ScanOperation_Type_DOUBLE:
						cerr << * ((double*) data[column] + row) <<" ";
						break;
					case OperationTree::ScanOperation_Type_BOOL:
						cerr <<  * ((bool*) data[column] + row) <<" ";
						break;
					}
			}
			cerr << endl; 
	}

		cerr << "1 GROUPSENDER ASFSDFSDFSD\n";

		// cast columns to columns for which we computw hashes
		vector<void*> hashed_columns_data;

		cast_to_hash_columns(data, hashed_columns_data);
		cerr << "2 GROUPSENDER ASFSDFSDFSD\n";
	
		int32* hashes = count_hashes(hashed_columns_data, hash_column_types_, rows);
		cerr << "3 GROUPSENDER ASFSDFSDFSD\n";

		scatter_data_into_buckets(data, rows, hashes);
		cerr << "4 GROUPSENDER ASFSDFSDFSD\n";

		cerr << "GROUPSENDER ASFSDFSDFSD\n";
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
				int who = Layers::get_real_node_number(1 - Layers::get_my_layer(), i);
				nei_ -> SendPacket(who, NULL, 0);
	//	send_bucket(i);
	}
	
	// this is dummy return value
	return std::vector<void*>();
}

}
