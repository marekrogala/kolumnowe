#ifndef MENGINE_H
#define MENGINE_H

#include "Operation.h"
#include "operations.pb.h"
#include "debug.h"
#include <iostream>
#include <vector>

#define BUFF_SIZE 2

using namespace std;

namespace Engine {


    class MEngine {

        public:
            MEngine(NodeEnvironmentInterface * nei, const OperationTree::Operation &operation, int max_rows);
            void run();

        private:
            NodeEnvironmentInterface * nei_;
						const OperationTree::Operation &operation_;
            Operation * root_operation_;
            int max_rows_;
			bool consume;
    };

}


#endif
