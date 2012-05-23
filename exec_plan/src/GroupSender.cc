#include <cassert>
#include <stdlib.h>
#include "GroupSender.h"

namespace Engine {

GroupSender::GroupSender(NodeEnvironmentInterface *node_env, Operation *source, const OperationTree::GroupByOperation &node) : 
	node_env_(node_env), source_(source), node_(node) {

	}

int32* countHashes(vector<void*> &data, int rows) {
	
}

std::vector<void*> GroupSender::pull(int &rows) {
  sleep(4);
	int nrows = 0;
	vector<void*> data;
	
	//data = source_->pull(rows);
	while (nrows > 0) {
		
	}
	
	char buff[20];
  sprintf(buff, "HEY from %d", node_env_->my_node_number());
  data.push_back((void*) buff);
  
  //serializacja
  std::cout << "Other layer " << CountNodesInOtherLayer(node_env_)<<std::endl;
  for (int i = 0 ; i < CountNodesInOtherLayer(node_env_); ++i) {
    std::cout<<"Sending..."<<std::endl;
      SendPacket(node_env_, i, buff, strlen(buff)+1);
  }

	// this is dummy return value
	return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupSender::init() {

	assert(false);
}

}
