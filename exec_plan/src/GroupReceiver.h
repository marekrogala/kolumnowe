#ifndef GROUP_RECEIVER
#define GROUP_RECEIVER

#include "node_environment.h"
#include "Operation.h"
#include "UniversalHashMap.h"
#include "RealUniversalHashMap.h"

namespace Engine {

class GroupReceiver : public Operation {

	private:
		NodeEnvironmentInterface* node_env_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;
		int worker_id;
		const OperationTree::GroupByOperation &node_;

	public:
		std::vector<void*> pull(int &rows);

		GroupReceiver(NodeEnvironmentInterface *node_env, OperationTree::GroupByOperation &node);

		std::vector<OperationTree::ScanOperation_Type> init();
};

}

#endif // GROUP_RECEIVER
