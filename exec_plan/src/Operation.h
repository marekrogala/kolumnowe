#ifndef ENGINE_OPERATION_H
#define ENGINE_OPERATION_H

#include <vector>
#include <utility>
#include <algorithm>
#include "operations.pb.h"
#include "node_environment.h"

using namespace std;


namespace Engine {
	  class Operation;
		typedef pair<vector<OperationTree::ScanOperation_Type>, Operation*> InitRes;
		typedef vector<OperationTree::ScanOperation_Type> Types;
    class Operation {
        public:
            virtual vector<void*> pull(int &rows) = 0;
            virtual InitRes init(bool &group_flag) { } 
            virtual OperationTree::ScanOperation_Type get_source_type(int idx) {
            	assert(false);
            }
            virtual void* get_data(int idx) {
            	assert(false);
            }


        protected:
            Operation(NodeEnvironmentInterface * nei) : nei_(nei) {}
            NodeEnvironmentInterface * nei_;
    };
}

#endif
