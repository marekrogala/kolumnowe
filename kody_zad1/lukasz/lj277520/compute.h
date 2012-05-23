#ifndef COMPUTE_H
#define COMPUTE_H

#include "dboperation.h"
#include "columnStruct.h"
#include "dbexpression.h"

class Compute : public DBOperation {
	private:
		DBOperation* source;
		DBExpression** expr;	
//		columnType* columnsTypes;
//		int outputSize;
	public:
		Compute(DBOperation* source, int outputSize, columnType* columnsTypes, DBExpression** expr);
		~Compute();

		virtual int getOutput(int number, ColumnStruct** destination);
};

#endif
