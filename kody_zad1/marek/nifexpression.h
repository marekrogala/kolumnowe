#ifndef NIFEXPRESSION_H_
#define NIFEXPRESSION_H_

#include "proto/operations.pb.h"
#include "ntypedexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

template<ColType T>
class NIfExpression : public NTypedExpression<T> {
  public:
    virtual ~NIfExpression();
    NIfExpression(NExpression * condition,
    		NExpression * optionTrue,
    		NExpression * optionFalse);
    virtual std::string toString() const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const;
  private:
	  NTypedExpression<BOOL> * condition_;
	  NTypedExpression<T> * optionTrue_;
	  NTypedExpression<T> * optionFalse_;
};

#endif

