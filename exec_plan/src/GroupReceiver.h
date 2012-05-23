#ifndef GROUP_RECEIVER
#define GROUP_RECEIVER

#include "node_environment.h"
#include "Operation.h"
#include "UniversalHashMap.h"
#include "RealUniversalHashMap.h"

class GroupReceiver {

	private:
		NodeEnvironmentInterface* node_env_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;
		int worker_id;

	public:
		std::vector<void*> pull(int &rows);
};

#endif // GROUP_RECEIVER
