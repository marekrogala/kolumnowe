#include <cassert>
#include <iostream>

#include "GroupReceiver.h"
#include "BlockSerializer.h"

namespace Engine {

GroupReceiver::GroupReceiver(NodeEnvironmentInterface *node_env, OperationTree::GroupByOperation &node) : 
	Operation(node_env), node_env_(node_env), node_(node) {
	}

std::vector<void*> GroupReceiver::pull(int &rows) {

	char *data;
	size_t data_len;
  

  	std::cout<<"Receiving..."<<std::endl;

 	while(true){
		data = node_env_->ReadPacketBlocking(&data_len);
	
		BlockSerializer serializer;
		serializer.deserializeBlock(source_types_, data_len, data, buffers_);
	
		std::cerr << data << std::endl;
  	}

	return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
