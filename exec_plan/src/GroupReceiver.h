#ifndef GROUP_RECEIVER
#define GROUP_RECEIVER

#include "operations.pb.h"
#include "UniversalHashmap.h"
#include "RealUniversalHashmap.h"
#include "layers.h"

namespace Engine {

class GroupReceiver : public Operation {

	private:
		NodeEnvironmentInterface* nei_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;
		int worker_id;
		const OperationTree::GroupByOperation &node_;
		std::vector<void*>  buffers_;

	public:
		std::vector<void*> pull(int &rows);

		GroupReceiver(NodeEnvironmentInterface *nei, OperationTree::GroupByOperation &node);

		std::vector<OperationTree::ScanOperation_Type> init();
};

}

#endif // GROUP_RECEIVER
