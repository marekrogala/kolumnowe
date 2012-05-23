#include "GroupReceiver.h"

namespace Engine {

GroupReceiver(NodeEnvironmentInterface *node_env, OperationTree::GroupByOperation &node) : 
	node_env_(node_env), node_(node) {

	}

std::vector<void*> GroupReceiver::pull(int &rows) {


}

}
