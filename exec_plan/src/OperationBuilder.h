/*
 * OperationBuilder.h
 *
 *  Created on: 19 Mar 2012
 *      Author: maciek
 */

#ifndef OPERATIONBUILDER_H_
#define OPERATIONBUILDER_H_

#include "operations.pb.h"
#include "Operation.h"
#include "ScanOperation.h"
#include "ComputeOperation.h"
#include "FilterOperation.h"
#include "GroupByOperation.h"
#include "MemoryManager.h"
#include "debug.h"
#include "node_environment/node_environment.h"

namespace Engine {

class OperationBuilder {
public:
	static Operation* build(NodeEnvironmentInterface * nei, const OperationTree::Operation &operation, MemoryManager * mem_manager);

};

}
#endif /* OPERATIONBUILDER_H_ */
