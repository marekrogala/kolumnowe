#ifndef ENGINE_COMPUTE_OPERATION_H
#define ENGINE_COMPUTE_OPERATION_H

#include "OperationBuilder.h"
#include "server.h"
#include "operations.pb.h"
#include "Operation.h"
#include "ExpressionNode.h"
#include "MemoryManager.h"
#include "debug.h"

namespace Engine {
    class ComputeOperation : public Operation{
        public:
            ComputeOperation(Server * server, const OperationTree::ComputeOperation & node, MemoryManager * mem_manager);
            vector<void*> pull(int &rows);
            vector<OperationTree::ScanOperation_Type> init();
            OperationTree::ScanOperation_Type get_source_type(int idx);
            void* get_data(int idx);
        private:
            vector<void*> data_;
            Server * server_;
            Operation * source_;
            vector<OperationTree::ScanOperation_Type> source_types_;
            vector<ExpressionNode *> expressions_;
            const OperationTree::ComputeOperation & node_;
            MemoryManager * mem_manager_;
    };

};


#endif
