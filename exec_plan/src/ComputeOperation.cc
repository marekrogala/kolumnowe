#include "ComputeOperation.h"
#include <iostream>

using namespace std;

namespace Engine {

OperationTree::ScanOperation_Type ComputeOperation::get_source_type(int idx) {
	return source_types_[idx];
}

ComputeOperation::ComputeOperation(Server * server, const OperationTree::ComputeOperation & node, MemoryManager * mem_manager):
		server_(server), node_(node), mem_manager_(mem_manager) {
}

vector<OperationTree::ScanOperation_Type> ComputeOperation::init() {
	if (debug) cerr << "ComputeOperation::init()" << endl;
    int n = node_.expressions_size();

    source_ = OperationBuilder::build(server_, node_.source(), mem_manager_);
    source_types_ = source_ -> init();

    vector<OperationTree::ScanOperation_Type> res;
    for(int i = 0; i < n; ++i) {
        expressions_.push_back(new ExpressionNode(&(node_.expressions(i)), this, mem_manager_));
        res.push_back(expressions_.back() -> init());
    }
    return res;
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

