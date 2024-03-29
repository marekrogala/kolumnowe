/*
 * FilterOperation.h
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#ifndef FILTEROPERATION_H_
#define FILTEROPERATION_H_

#include "OperationBuilder.h"
#include "Operation.h"
#include "server.h"
#include "operations.pb.h"
#include "ExpressionNode.h"
#include "MemoryManager.h"
#include "debug.h"

namespace Engine {
class ExpressionNode;

class FilterOperation: public Operation {
	public:
	   FilterOperation(Server * server, const OperationTree::FilterOperation & node, MemoryManager * mem_manager);
	            vector<void*> pull(int &rows);
	            vector<OperationTree::ScanOperation_Type> init();
	            OperationTree::ScanOperation_Type get_source_type(int idx);
	            void* get_data(int idx);
	        private:
	            vector<void*> data_;
	            vector<void*> res_;
	            vector<void*> buffered_;
	            int buffered_size_;
	            Server * server_;
	            Operation * source_;
	            vector<OperationTree::ScanOperation_Type> source_types_;
	            ExpressionNode * expr_;
	            const OperationTree::FilterOperation & node_;
	            MemoryManager * mem_manager_;
};

}

#endif /* FILTEROPERATION_H_ */
