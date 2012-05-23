#ifndef EXPRESSION_OPER_H
#define EXPRESSION_OPER_H

#include "dbexpression.h"

template<class TA, class TB, class T, typename O>
class Expression_oper : public DBExpression {
	private:
		DBExpression* expr1;
		DBExpression* expr2;
	public:
		Expression_oper(DBExpression* expr1, DBExpression* expr2);
		virtual ~Expression_oper();
		virtual int getResult(ColumnStruct* columns, void** destination);
};

template<class TA, class TB, class T, typename O>
Expression_oper<TA, TB, T, O>::Expression_oper(DBExpression* expr1, DBExpression* expr2) : expr1(expr1), expr2(expr2) {
}

template<class TA, class TB, class T, typename O>
Expression_oper<TA, TB, T, O>::~Expression_oper() {
	delete expr1;
	delete expr2;
}

template<class TA, class TB, class T, typename O>
int Expression_oper<TA, TB, T, O>::getResult(ColumnStruct* columns, void** destination) {
	void* e1;
	void* e2;
	int  n = expr1->getResult(columns, &e1);
	expr2->getResult(columns, &e2);
	
	T* result = new T[n];

	oper<TA, TB, T, O>(n, (TA*)e1, (TB*)e2, result);

	delete[] (TA*)e1;
	delete[] (TB*)e2;
	*destination = result;
	return n;
}

#endif
