#ifndef EXPRESSION_oper1_H
#define EXPRESSION_oper1_H

#include "dbexpression.h"

template<class TA, class T, typename O>
class Expression_oper1 : public DBExpression {
	private:
		DBExpression* expr1;
	public:
		Expression_oper1(DBExpression* expr1);
		virtual ~Expression_oper1();
		virtual int getResult(ColumnStruct* columns, void** destination);
};

template<class TA, class T, typename O>
Expression_oper1<TA, T, O>::Expression_oper1(DBExpression* expr1) : expr1(expr1) {
}

template<class TA, class T, typename O>
Expression_oper1<TA, T, O>::~Expression_oper1() {
	delete expr1;
}

template<class TA, class T, typename O>
int Expression_oper1<TA, T, O>::getResult(ColumnStruct* columns, void** destination) {
	void* e1;
	int  n = expr1->getResult(columns, &e1);
	
	T* result = new T[n];

	oper1<TA, T, O>(n, (TA*)e1, result);

	delete[] (TA*)e1;
	*destination = result;
	return n;
}

#endif
