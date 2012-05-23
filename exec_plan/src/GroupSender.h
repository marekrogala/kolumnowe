#ifndef GROUP_SENDER
#define GROUP_SENDER

#include <vector>

#include "Operation.h"
#include "node_environment.h"
#include "UniversalHashmap.h"
#include "RealUniversalHashmap.h"

class GroupSender : public Operation {

	private:
		NodeEnvironmentInterface* node_env_;
		Operation *source_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;		

	public:
		std::vector<void*> pull(int &rows);
};

#endif // GROUP_SENDER
