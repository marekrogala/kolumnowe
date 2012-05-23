#include "ScanOperation.h"
#include <iostream>

using namespace std;

namespace Engine {

vector<OperationTree::ScanOperation_Type> ScanOperation::init() {
    if (debug) cerr << "ScanOperation::init()" << endl;
    int n = node_.column_size();
    for(int i = 0; i < n; ++i) {
        columns_.push_back(node_.column(i));
        types_.push_back(node_.type(i));
    }
    return types_;
}

vector<void*> ScanOperation::pull(int &rows) {
    if (debug) cerr << "ScanOperation pull " << rows << endl;
    
    if (buffers_.size() == 0) init_buffers();
    int n = columns_.size();
    vector<void*> res;

    int max_rows = rows;

    for(int i = 0; i < n; ++i) {
        int x;
        switch(types_[i]) {
            case OperationTree::ScanOperation_Type_INT:
                x = server_ -> GetInts(columns_[i], max_rows, (int32*)buffers_[i]);
                rows = x;
                res.push_back(buffers_[i]);
                break;

            case OperationTree::ScanOperation_Type_DOUBLE:
                x = max_rows;
                x = server_ -> GetDoubles(columns_[i], max_rows, (double*)buffers_[i]);
                rows = x;
                res.push_back(buffers_[i]);
                break;

            case OperationTree::ScanOperation_Type_BOOL:
                x = server_ -> GetByteBools(columns_[i], max_rows, (bool*)buffers_[i]);
                rows = x;
                res.push_back(buffers_[i]);
                break;
        }
    }
    return res;
}


void ScanOperation::init_buffers() {
    int n = columns_.size();
    for(int i = 0; i < n; ++i) {
        buffers_.push_back(mem_manager_ -> allocate_normal());
    }
}

void ScanOperation::free_buffers() {
	assert(false);
}

ScanOperation::ScanOperation(Server * server, const OperationTree::ScanOperation &node, MemoryManager * mem_manager) :
		node_(node), server_(server), mem_manager_(mem_manager)	 {
}

}


