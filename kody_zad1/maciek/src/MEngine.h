#ifndef MENGINE_H
#define MENGINE_H

#include "Operation.h"
#include "operations.pb.h"
#include "server.h"
#include "Operation.h"
#include "debug.h"
#include <iostream>
#include <vector>

using namespace std;

namespace Engine {
    class MEngine {

        public:
            MEngine(const OperationTree::Operation &operation, Server * server, int max_rows);
            void run();

        private:
            Operation * root_operation_;
            Server * server_;
            int max_rows_;
    };

}


#endif
