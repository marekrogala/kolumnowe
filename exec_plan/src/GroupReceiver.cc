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

	data = nei_->ReadPacketBlocking(&data_len);
	
	BlockSerializer serializer;
	rows = serializer.deserializeBlock(source_types_, data_len, data, buffers_);

	return buffers_;
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
