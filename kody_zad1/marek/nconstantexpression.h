#ifndef NCONSTANTEXPRESSION_H_
#define NCONSTANTEXPRESSION_H_

#include "proto/operations.pb.h"
#include "ntypedexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

template<ColType T>
class NConstantExpression : public NTypedExpression<T> {
  public:
    virtual ~NConstantExpression() {}
    NConstantExpression(typename TypeTraits<T>::T constant_value);
    virtual std::string toString() const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const;
  private:
    typename TypeTraits<T>::T constant_value_;
};

#endif

