#include <cassert>
#include <stdlib.h>

#include "GroupSender.h"
#include "BlockSerializer.h"
#include "layers.h"

namespace Engine {

GroupSender::GroupSender(NodeEnvironmentInterface *nei, Operation *source, const OperationTree::GroupByOperation &node) : 
	Operation(nei), nei_(nei), source_(source), node_(node) {

	}

int32* GroupSender::count_hashes(vector<void*> &data, int rows) {
	// dummy implementation (hashes equal to 0)
	int32* res = new int32[rows]();
	return res;
}

void GroupSender::scatter_data_into_buckets(vector<void*> data, int rows, int32* hashes) {
	for (int row = 0; row < rows; ++row) {
		int bucket = hashes[row] % CountNodesInOtherLayer(nei_);
		for (int column = 0, columns = data.size(); column < columns; ++columns)
			switch (source_types_[columns]) {
				case ScanOperation_Type_INT:
					std::fill_n( (int*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((int*) data[columns] + row));
					break;
				case ScanOperation_Type_DOUBLE:
					std::fill_n( (double*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((double*) data[columns] + row));
				case ScanOperation_Type_BOOL:
					std::fill_n( (bool*) buckets_[bucket][column] + buckets_load_[bucket], 1, * ((bool*) data[columns] + row));
			}
		buckets_load_[bucket]++;
	}
}

bool GroupSender::bucket_ready_to_send(int bucket) {
	// only example implementation
	return buckets_load_[bucket] > 1000;
}

vector<void*> GroupSender::pull(int &rows) {


  	sleep(4);
	
	int nrows = 0;
	vector<void*> data;

	do {
		data = source_->pull(rows);
	
		int32* hashes = count_hashes(data, rows);

		scatter_data_into_buckets(data, rows, hashes);

		for(int i = 0, buckests_no = buckets_.size(); i < buckests_no; ++i)
			if (bucket_ready_to_send(i)) {
				
				// serialize data
				char* serializedData;
				BlockSerializer serializer;
				int message_len = serializer.serializeBlock(source_types_, buckets_[i], buckets_load_, &serializedData);

				// send data
				SendPacket(nei_, i, serializedData, message_len);

				// reset this bucket
				buckets_load_[i] = 0;
		   }



	} while (rows > 0);
	
	/*char buff[20];
  	sprintf(buff, "HEY from %d", nei_->my_node_number());
 	data.push_back((void*) buff);
  
  	std::cout << "Other layer " << CountNodesInOtherLayer(nei_)<<std::endl;
  	for (int i = 0 ; i < CountNodesInOtherLayer(nei_); ++i) {
    	std::cout<<"Sending..."<<std::endl;
      	SendPacket(nei_, i, buff, strlen(buff)+1);
  	}*/	

	// this is dummy return value
	return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupSender::init() {

	assert(false);
}


