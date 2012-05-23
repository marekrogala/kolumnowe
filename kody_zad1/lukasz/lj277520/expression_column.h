#ifndef EXPRESSION_COLUMN_H
#define EXPRESSION_COLUMN_H

#include "dbexpression.h"

class Expression_column : public DBExpression {
	private:
		int columnId;
	public:
		Expression_column(int columnId);
		~Expression_column();

		virtual int getResult(ColumnStruct* columns, void** destination);
};

#endif
