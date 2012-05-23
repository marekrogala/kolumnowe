#ifndef DBEXPRESSION_H
#define DBEXPRESSION_H

#include <stdint.h>
#include <math.h>

#include "columnStruct.h"

typedef int32_t int32;

class DBExpression {
	public:
		virtual ~DBExpression() { }
		virtual int getResult(ColumnStruct* columns, void** destination) = 0;
};

template<class TA, class TB, class T, typename O>
void oper(int n, TA* a, TB* b, T* r) {
	O op;
	for(int i = 0; i < n; ++i) {
		r[i] = op(a[i],b[i]);
	}
}

template<class TA, class T, typename O>
void oper1(int n, TA* a, T* r) {
	O op;
	for(int i = 0; i < n; ++i) {
		r[i] = op(a[i]);
	}
}

template<class TA, class TB, class T>
struct addS {
	inline T operator()(TA a, TB b) const {
		return a+b;
	}
};

template<class TA, class TB, class T>
struct subS {
	inline T operator()(TA a, TB b) const {
		return a-b;
	}
};

template<class TA, class TB, class T>
struct mulS {
	inline T operator()(TA a, TB b) const {
		return a*b;
	}
};

template<class TA, class TB, class T>
struct divS {
	inline T operator()(TA a, TB b) const {
		return (T)a/(T)b;
	}
};

template<class TA, class TB, class T>
struct ltS {
	inline T operator()(TA a, TB b) const {
		return a < b;
	}
};

template<class TA, class TB, class T>
struct gtS {
	inline T operator()(TA a, TB b) const {
		return a > b;
	}
};

template<class TA, class TB, class T>
struct eqS {
	inline T operator()(TA a, TB b) const {
		return a == b;
	}
};

template<class TA, class TB, class T>
struct neqS {
	inline T operator()(TA a, TB b) const {
		return a != b;
	}
};

template<class TA, class TB, class T>
struct andS {
	inline T operator()(TA a, TB b) const {
		return a && b;
	}
};

template<class TA, class TB, class T>
struct orS {
	inline T operator()(TA a, TB b) const {
		return a || b;
	}
};

template<class TA, class T>
struct negS {
	inline T operator()(TA a) const {
		return (-1)*a;
	}
};

template<class TA, class T>
struct logS {
	inline T operator()(TA a) const {
		return log(a);
	}
};

template<class TA, class T>
struct notS {
	inline T operator()(TA a) const {
		return !a;
	}
};

#endif
