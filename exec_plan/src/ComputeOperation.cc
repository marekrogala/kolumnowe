#include "ComputeOperation.h"
#include <iostream>

using namespace std;

namespace Engine {

OperationTree::ScanOperation_Type ComputeOperation::get_source_type(int idx) {
	return source_types_[idx];
}

ComputeOperation::ComputeOperation(NodeEnvironmentInterface * nei, const OperationTree::ComputeOperation & node, MemoryManager * mem_manager):
		Operation(nei), node_(node), mem_manager_(mem_manager) {
}

InitRes ComputeOperation::init(bool& group_flag) {
	if (debug) cerr << "ComputeOperation::init()" << endl;
		source_ = OperationBuilder::build(nei_, node_.source(), mem_manager_);
    InitRes r = source_ -> init(group_flag);
		source_types_ = r.first;
		Operation *node = r.second;

    int n = node_.expressions_size();

        vector<OperationTree::ScanOperation_Type> res;
    for(int i = 0; i < n; ++i) {
        expressions_.push_back(new ExpressionNode(&(node_.expressions(i)), this, mem_manager_));
        res.push_back(expressions_.back() -> init());
    }

		if (group_flag) {
			source_ = node;
			return make_pair(res, this);
		} else {
			return make_pair(res, node);
		}
}

vector<void*> ComputeOperation::pull(int &rows) {
	if(debug) cerr << "ComputeOperation::pull " << rows << endl;
	data_ = source_ -> pull(rows);

	vector<void*> res;

	for(int i = 0; i < expressions_.size(); ++i) {
		res.push_back(expressions_[i] ->pull(rows));
	}

	return res;
}

void* ComputeOperation::get_data(int idx) {
	return data_[idx];
}

}

