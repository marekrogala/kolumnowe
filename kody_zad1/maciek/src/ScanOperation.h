#ifndef ENGINE_SCAN_OPERATION_H
#define ENGINE_SCAN_OPERATION_H

#include "server.h"
#include "operations.pb.h"
#include "Operation.h"
#include <vector>
#include "MemoryManager.h"
#include "debug.h"

using namespace std;

namespace Engine {
    class ScanOperation : public Operation {
        public:
            ScanOperation(Server * server, const OperationTree::ScanOperation &node, MemoryManager * mem_manager_);
            vector<void*> pull(int &rows);
            vector<OperationTree::ScanOperation_Type> init();

        private:
            void init_buffers();
            void free_buffers();

        private:
            const OperationTree::ScanOperation &node_;
            Server *server_;
            vector<void*> buffers_;
            vector<int> columns_;
            vector<OperationTree::ScanOperation_Type> types_;
            MemoryManager * mem_manager_;
    };
};

#endif
