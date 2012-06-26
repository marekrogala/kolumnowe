/*
 * GroupByOperation.h
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#ifndef GROUPBYOPERATION_H_
#define GROUPBYOPERATION_H_

#include "Operation.h"
#include "OperationBuilder.h"
#include "node_environment.h"
#include "operations.pb.h"
#include "ExpressionNode.h"
#include "MemoryManager.h"
#include "UniversalHashmap.h"
#include "debug.h"
#include "RealUniversalHashmap.h"
#include "GroupSender.h"
#include "GroupReceiver.h"

namespace Engine {

class GroupByOperation: public Engine::Operation {

public:
	GroupByOperation(NodeEnvironmentInterface * nei, const OperationTree::GroupByOperation & node, MemoryManager * mem_manager);
    vector<void*> pull(int &rows);
		InitRes init(bool &group_flag);
private:
            Operation * source_;
            vector<OperationTree::ScanOperation_Type> source_types_;
            const OperationTree::GroupByOperation & node_;
            MemoryManager * mem_manager_;
            UniversalMap * map_;
            int32 * count_buffer_;
            vector<void*> res_;
            vector<OperationTree::ScanOperation_Type> res_types_;
            vector<OperationTree::ScanOperation_Type> hash_signature_;
            int res_size_;
            bool first_time_;
            void first_pull();
						void init_hashmap();
};

}

#endif /* GROUPBYOPERATION_H_ */
