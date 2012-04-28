/*
 * OperationBuilder.cpp
 *
 *  Created on: 19 Mar 2012
 *      Author: maciek
 */

#include "OperationBuilder.h"
#include "debug.h"

Engine::Operation* Engine::OperationBuilder::build(Server * server, const OperationTree::Operation &operation, MemoryManager * mem_manager) {
		if (operation.has_scan()) {
			return new ScanOperation(server, operation.scan(), mem_manager);
		}

		if (operation.has_compute()) {
			return new ComputeOperation(server, operation.compute(), mem_manager);
		}

		if (operation.has_filter()) {
			return new FilterOperation(server, operation.filter(), mem_manager);
		}

		if (operation.has_group_by()) {
			return new GroupByOperation(server, operation.group_by(), mem_manager);
		}

		if (debug) assert(false);
	}

