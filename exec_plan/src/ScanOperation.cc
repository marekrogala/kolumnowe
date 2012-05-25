#include "ScanOperation.h"
#include <iostream>
#include "ExpressionNode.h"

using namespace std;

namespace Engine {

vector<OperationTree::ScanOperation_Type> ScanOperation::init() {
    if (debug) cerr << "ScanOperation::init()" << endl;

	int numberOfNodes = nei_ -> nodes_count();
	for(int i = nei_ -> my_node_number(); i < numberOfFiles_; i+=numberOfNodes) {
		dataSources_.push_back(nei_->OpenDataSourceFile(i));
	}
	currentFile_ = dataSources_.begin();

    int n = node_.column_size();
    for(int i = 0; i < n; ++i) {
        columns_.push_back(node_.column(i));
        types_.push_back(node_.type(i));
    }
    return types_;
}

vector<void*> ScanOperation::pull(int &rows) {
    cerr << "ScanOperation pull " << rows << endl;
    
    if (buffers_.size() == 0) init_buffers();
    int n = columns_.size();
    vector<void*> res;

    int max_rows = rows;
    int scanned = 0;

    for(int i = 0; i < n; ++i) {
    	int rows_left = rows;
    	switch (types_[i]) {

			case SINT :
			{
				int32 * buff = static_cast<int32*>(buffers_[i]);
				while (currentFile_ != dataSources_.end()) {
					int x = (*currentFile_) -> GetInts(columns_[i], rows_left, buff);
					cerr << "YYYYYYY " << x << endl;
					if (x == 0) {
						currentFile_++;
					} else {
						buff += x;
						rows_left -= x;
						break;
					}
				}
				scanned = rows - rows_left;
				break;
			}

			case SDOUBLE :
			{
				double * buff = static_cast<double*>(buffers_[i]);
				while (currentFile_ != dataSources_.end()) {
					int x = (*currentFile_) -> GetDoubles(columns_[i], rows_left, buff);
					if (x == 0) {
						currentFile_++;
					} else {
						buff += x;
						rows_left -= x;
						break;
					}
				}
				scanned = rows - rows_left;
				break;
			}

			case SBOOL :
			{
				bool * buff = static_cast<bool*>(buffers_[i]);
				while (currentFile_ != dataSources_.end()) {
					int x = (*currentFile_) -> GetByteBools(columns_[i], rows_left, buff);
					if (x == 0) {
						currentFile_++;
					} else {
						buff += x;
						rows_left -= x;
						break;
					}
				}
				scanned = rows - rows_left;
				break;
			}
    	}
    	res.push_back(buffers_[i]);
    }

    rows = scanned;
    cerr << "SCANNED " << rows << " " << numberOfFiles_ << endl;
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


