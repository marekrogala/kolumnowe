/*
 * ExpressionNode.h
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#ifndef EXPRESSIONNODE_H_
#define EXPRESSIONNODE_H_
#include "server.h"
#include "operations.pb.h"
#include "Operation.h"
#include "ComputeOperation.h"
#include "MemoryManager.h"
#include "debug.h"

namespace Engine {

const OperationTree::ScanOperation_Type SBOOL = OperationTree::ScanOperation_Type_BOOL;
const OperationTree::ScanOperation_Type SINT = OperationTree::ScanOperation_Type_INT;
const OperationTree::ScanOperation_Type SDOUBLE = OperationTree::ScanOperation_Type_DOUBLE;

class ComputeOperation;

class ExpressionNode {
	private:
	vector<void*> buffers_;
	const OperationTree::Expression * expr_;
	Operation * oper_;
	vector<ExpressionNode*> t_;
	vector<OperationTree::ScanOperation_Type> types_;
	MemoryManager * mem_manager_;

	public:
	ExpressionNode(const OperationTree::Expression * expr, Operation * oper, MemoryManager * mem_manager);
	OperationTree::ScanOperation_Type init();
	void* pull(int rows);
};

}

#endif /* EXPRESSIONNODE_H_ */
