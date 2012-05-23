#include <cstring>

#include "dboperation.h"

#include <iostream>

DBOperation::DBOperation(int outputSize, columnType* columnsTypes) : outputSize(outputSize), columnsTypes(columnsTypes) {
}

DBOperation::~DBOperation() {
	delete[] columnsTypes;
}

int DBOperation::copyColumnsTypes(columnType** destination) {
	columnType* types = new columnType[outputSize];
	memcpy(types, columnsTypes, outputSize*sizeof(columnType));
	*destination = types;
	return outputSize;
}
