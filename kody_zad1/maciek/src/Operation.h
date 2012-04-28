#ifndef ENGINE_OPERATION_H
#define ENGINE_OPERATION_H

#include <vector>
#include "operations.pb.h"

using namespace std;

namespace Engine {
    class Operation {
        public:
            virtual vector<void*> pull(int &rows) = 0;
            virtual vector<OperationTree::ScanOperation_Type> init() = 0;
            virtual OperationTree::ScanOperation_Type get_source_type(int idx) {
            	assert(false);
            }
            virtual void* get_data(int idx) {
            	assert(false);
            }
    };
}

#endif
