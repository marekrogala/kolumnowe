/*
 * OperationBuilder.cpp
 *
 *  Created on: 19 Mar 2012
 *      Author: maciek
 */

#include "OperationBuilder.h"
#include "node_environment.h"
#include "debug.h"

Engine::Operation* Engine::OperationBuilder::build(NodeEnvironmentInterface * nei,
	    const OperationTree::Operation &operation, MemoryManager * mem_manager) {
		if (operation.has_scan()) {
			return new ScanOperation(nei, operation.scan(), mem_manager);
		}

		if (operation.has_compute()) {
			return new ComputeOperation(nei, operation.compute(), mem_manager);
		}

		if (operation.has_filter()) {
			return new FilterOperation(nei, operation.filter(), mem_manager);
		}

		if (operation.has_group_by()) {
			return new GroupByOperation(nei, operation.group_by(), mem_manager);
		}

		if (debug) assert(false);
	}

