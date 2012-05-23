#ifndef ENGINE_SCAN_OPERATION_H
#define ENGINE_SCAN_OPERATION_H

#include "server.h"
#include "operations.pb.h"
#include "Operation.h"
#include <vector>
#include "MemoryManager.h"
#include "debug.h"

#include "node_environment.h"

using namespace std;

namespace Engine {
    class ScanOperation : public Operation {
        public:
            ScanOperation(NodeEnvironmentInterface* nei, int numberOfFiles, const OperationTree::ScanOperation &node, MemoryManager * mem_manager_);
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
			NodeEnvironmentInterface* nei_;
			int numberOfFiles_;
			vector<DataSourceInterface*> dataSources_;
			vector<DataSourceInterface*>::iterator currentFile_;
    };
};

#endif
