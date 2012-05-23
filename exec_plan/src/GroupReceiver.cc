#include <cassert>
#include <iostream>

#include "GroupReceiver.h"

namespace Engine {

GroupReceiver::GroupReceiver(NodeEnvironmentInterface *node_env, OperationTree::GroupByOperation &node) : 
	node_env_(node_env), node_(node) {

	}

std::vector<void*> GroupReceiver::pull(int &rows) {

		char *data;
		size_t data_len;
  
  sleep(10);

  std::cout<<"Receiving..."<<std::endl;

  while(true){
		data = node_env_->ReadPacketBlocking(&data_len);
		std::cerr << data << std::endl;
  }
		return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupReceiver::init() {
	assert(false);
}

}
