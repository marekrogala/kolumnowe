#ifndef GROUP_SENDER
#define GROUP_SENDER

#include <vector>

#include "Operation.h"
#include "node_environment.h"
#include "UniversalHashmap.h"
#include "RealUniversalHashmap.h"

class GroupSender : public Operation {

	private:
		NodeEnvironmentInterface *node_env_;
		Operation *source_;
		std::vector<OperationTree::ScanOperation_Type> source_types_;	
		

		// node of the original tree corresponding
		const OperationTree::GroupByOperation & node_;

		// we use hash function to scatter data into buckets
		// when there is enough rows in a bucket, data is sent to 
		// corresponding worker in second group
		std::vector<std::vector<void*> > buckets_;	


	public:
		GroupSender(NodeEnvironmentInterface *node_env, Operation *source, const OperationTree::GroupByOperation &node) :
			node_env_(node_env), source_(source), node_(node) {}

		std::vector<void*> pull(int &rows);
};

#endif // GROUP_SENDER
