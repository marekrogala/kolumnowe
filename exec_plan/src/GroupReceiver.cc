#include <cassert>
#include <iostream>

#include "GroupReceiver.h"
#include "BlockSerializer.h"

namespace Engine {

GroupReceiver::GroupReceiver(NodeEnvironmentInterface *nei, 
		std::vector<OperationTree::ScanOperation_Type> source_types, Operation*source) : 
	Operation(nei), nei_(nei), source_types_(source_types), source_(source) {
	}

std::vector<void*> GroupReceiver::pull(int &rows) {
	cerr << "GroupReceiver::pull " << rows << endl;	
	char *data;
	size_t data_len;
	if (source_ != NULL)
		source_ -> pull(rows);

	std::cerr << "Receiving..." << std::endl;

	int eofs_to_be_received = Layers::count_nodes_in_other_layer();

	do {
	  data = nei_->ReadPacketBlocking(&data_len);
		cerr <<data_len << endl;
	  if(data_len == 0) eofs_to_be_received--;
	} while(data_len == 0 && eofs_to_be_received > 0);
	
	if(eofs_to_be_received == 0) {
	  rows = 0;
	  return std::vector<void*>();
	}
	
	BlockSerializer serializer;
	rows = serializer.deserializeBlock(source_types_, data_len, data, buffers_);

	// TODO - deallocate data block

	return buffers_;
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
