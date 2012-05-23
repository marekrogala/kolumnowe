#ifndef FILTER_H
#define FILTER_H

#include "dboperation.h"
#include "columnStruct.h"
#include "dbexpression.h"

class Filter : public DBOperation {
	private:
		DBOperation* source;
		DBExpression* expr;	
	public:
		Filter(DBOperation* source, int outputSize, columnType* columnsTypes, DBExpression* expr);
		~Filter();

		virtual int getOutput(int number, ColumnStruct** destination);
};

#endif
