#ifndef EXPRESSION_IF_H
#define EXPRESSION_IF_H

#include "dbexpression.h"

template<class T>
class Expression_if : public DBExpression {
	private:
		DBExpression* expri;
		DBExpression* expr1;
		DBExpression* expr2;
	public:
		Expression_if(DBExpression* expri, DBExpression* expr1, DBExpression* expr2);
		virtual ~Expression_if();

		virtual int getResult(ColumnStruct* columns, void** destination);
};

template<class T>
Expression_if<T>::Expression_if(DBExpression* expri, DBExpression* expr1, DBExpression* expr2) : expri(expri), expr1(expr1), expr2(expr2) {
}

template<class T>
Expression_if<T>::~Expression_if() {
	delete expri;
	delete expr1;
	delete expr2;
}

template<class T>
int Expression_if<T>::getResult(ColumnStruct* columns, void** destination) {
	void* r;
	int n = expri->getResult(columns, &r);
	void* e1;
	void* e2;
	expr1->getResult(columns, &e1);
	expr2->getResult(columns, &e2);
	bool* bresult = (bool*)r;
	T* t1 = (T*)e1;
	T* t2 = (T*)e2;
	T* result = new T[n];
	for(int i = 0; i < n; i++) {
		result[i] = bresult[i] ? t1[i] : t2[i];
	}
	delete[] bresult;
	delete[] (T*)e1;
	delete[] (T*)e2;
	*destination = result;
	return n;
}


#endif
