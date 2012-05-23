#include <cassert>

#include "GroupSender.h"

namespace Engine {

GroupSender::GroupSender(NodeEnvironmentInterface *node_env, Operation *source, const OperationTree::GroupByOperation &node) : 
	node_env_(node_env), source_(source), node_(node) {

	}

int32* countHashes(vector<void*> &data, int rows) {
	
}

std::vector<void*> GroupSender::pull(int &rows) {

	int rows = 0;
	vector<void*> data;

	//data = source_->pull(rows);
	while (rows > 0) {
		
	}

	// this is dummy return value
	return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupSender::init() {

	assert(false);
}

}
