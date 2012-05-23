#ifndef DBOPERATION_H
#define DBOPERATION_H

#include <stdint.h>

#include "columnStruct.h"

typedef int32_t int32;

class DBOperation {
	protected:
		columnType* columnsTypes;
		int outputSize;
		DBOperation(int outputSize, columnType* columnsTypes);
	public:
		virtual ~DBOperation();
		virtual int getOutput(int number, ColumnStruct** destination) = 0;
		int copyColumnsTypes(columnType** destination);
};

#endif
