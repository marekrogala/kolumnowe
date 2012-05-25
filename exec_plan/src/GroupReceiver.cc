#include <cassert>
#include <iostream>

#include "GroupReceiver.h"
#include "BlockSerializer.h"

namespace Engine {

GroupReceiver::GroupReceiver(NodeEnvironmentInterface *nei, OperationTree::GroupByOperation &node) : 
	Operation(nei), nei_(nei), node_(node) {
	}

std::vector<void*> GroupReceiver::pull(int &rows) {

	char *data;
	size_t data_len;

	std::cerr << "Receiving..." << std::endl;

	int eofs_to_be_received = CountNodesInOtherLayer(nei_);

	do {
	  data = nei_->ReadPacketBlocking(&data_len);
	  if(data_len == 0) eofs_to_be_received--;
	} while(data_len == 0 && eofs_to_be_received > 0);
	
	if(eofs_to_be_received == 0) {
	  rows = 0;
	  return NULL;
	}
	
	BlockSerializer serializer;
	rows = serializer.deserializeBlock(source_types_, data_len, data, buffers_);

	return buffers_;
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
