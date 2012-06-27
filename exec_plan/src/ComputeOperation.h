#ifndef ENGINE_COMPUTE_OPERATION_H
#define ENGINE_COMPUTE_OPERATION_H

#include "OperationBuilder.h"
#include "operations.pb.h"
#include "Operation.h"
#include "ExpressionNode.h"
#include "MemoryManager.h"
#include "node_environment.h"
#include "debug.h"

namespace Engine {
    class ComputeOperation : public Operation{
        public:
            ComputeOperation(NodeEnvironmentInterface * nei, const OperationTree::ComputeOperation & node, MemoryManager * mem_manager);
            vector<void*> pull(int &rows);
            OperationTree::ScanOperation_Type get_source_type(int idx);
						InitRes init(bool& group_flag);
            void* get_data(int idx);
        private:
            vector<void*> data_;
            Operation * source_;
            vector<OperationTree::ScanOperation_Type> source_types_;
            vector<ExpressionNode *> expressions_;
            const OperationTree::ComputeOperation & node_;
            MemoryManager * mem_manager_;
    };

};


#endif
