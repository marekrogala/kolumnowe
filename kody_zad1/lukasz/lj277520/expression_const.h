#ifndef EXPRESSION_CONST_H
#define EXPRESSION_CONST_H

#include "dbexpression.h"

template <class T>
class Expression_const : public DBExpression {
	private:
		T c;
	public:
		Expression_const(T c);
		~Expression_const();
		virtual int getResult(ColumnStruct* columns, void** destination);
};

template <class T>
Expression_const<T>::Expression_const(T c) : c(c) {
}

template <class T>
Expression_const<T>::~Expression_const() {
}

template <class T>
int Expression_const<T>::getResult(ColumnStruct* columns, void** destination) {
//	int n = columns->sizes[0];
	int n = columns->nrows;
	void* result = new T[n];
	for(int i = 0; i < n; i++) {
		((T*)result)[i] = c;
	}
	*destination = result;
	return n;
}

#endif
