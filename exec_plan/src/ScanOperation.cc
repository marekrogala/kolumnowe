#include "ScanOperation.h"
#include <iostream>

using namespace std;

namespace Engine {

vector<OperationTree::ScanOperation_Type> ScanOperation::init() {
    if (debug) cerr << "ScanOperation::init()" << endl;

	cerr << "DDDDDDDDDDDDDDDDD " << nei_ << endl;
	int numberOfNodes = nei_ -> nodes_count();
	cerr << "DDDDDDDDDDDDDDDDD " << nei_ << endl;
	for(int i = nei_ -> my_node_number(); i < numberOfFiles_; i+=numberOfNodes) {
		cerr << "OPENED " << endl;
		dataSources_.push_back(nei_->OpenDataSourceFile(i));
		cerr << "OPENED " << endl;
	}
	currentFile_ = dataSources_.begin();

	cerr << "fsdfsdffsd\n";
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
        int x = 0;
		if(currentFile_ == dataSources_.end()) {
			rows = 0;
			res.push_back(buffers_[i]);
		}
		else {
			switch(types_[i]) {
				case OperationTree::ScanOperation_Type_INT:
					while(x == 0 && currentFile_ != dataSources_.end()) {
						x = (*currentFile_) -> GetInts(columns_[i], max_rows, (int32*)buffers_[i]);
						if(x > 0) {
							rows = x;
							res.push_back(buffers_[i]);
						}
						else {
							currentFile_++;
							if(currentFile_ == dataSources_.end()) {
								rows = 0;
								res.push_back(buffers_[i]);
							}
						}
					}
					break;

				case OperationTree::ScanOperation_Type_DOUBLE:
					while(x == 0 && currentFile_ != dataSources_.end()) {
						x = (*currentFile_) -> GetDoubles(columns_[i], max_rows, (double*)buffers_[i]);
						if(x > 0) {
							rows = x;
							res.push_back(buffers_[i]);
						}
						else {
							currentFile_++;
							if(currentFile_ == dataSources_.end()) {
								rows = 0;
								res.push_back(buffers_[i]);
							}
						}
					}
					break;

				case OperationTree::ScanOperation_Type_BOOL:
					while(x == 0 && currentFile_ != dataSources_.end()) {
						x = (*currentFile_) -> GetByteBools(columns_[i], max_rows, (bool*)buffers_[i]);
						if(x > 0) {
							rows = x;
							res.push_back(buffers_[i]);
						}
						else {
							currentFile_++;
							if(currentFile_ == dataSources_.end()) {
								rows = 0;
								res.push_back(buffers_[i]);
							}
						}
					}
					break;
			}
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

ScanOperation::ScanOperation(NodeEnvironmentInterface* nei, const OperationTree::ScanOperation &node, MemoryManager * mem_manager) :
		Operation(nei), node_(node), mem_manager_(mem_manager)	 {
		numberOfFiles_ = node.number_of_files();
}

}


