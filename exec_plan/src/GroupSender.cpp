#include <cassert.h>

#include "GroupSender.h"

namespace Engine {

GroupSender(NodeEnvironmentInterface *node_env, Operation *source, const OperationTree::GroupByOperation &node) : node_env_(node_env), 
	source_(source), node_(node) {
	


}

std::vector<void*> GroupSender::pull(int &rows) {

	// this is dummy return value
	return std::vector<void*>();
}

std::vector<OperationTree::ScanOperation_Type> GroupSender::init() {
	assert(false);
}

}
