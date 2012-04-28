#ifndef NTYPEDEXPRESSION_H_
#define NTYPEDEXPRESSION_H_

#include "proto/operations.pb.h"
#include "nexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

template<ColType T>
class NTypedExpression : public NExpression {
  public:
    virtual ~NTypedExpression() {}
    virtual std::string toString() const;
    virtual ColType getType() const;
    NTypedExpression(){}
    void untypedCompute(int number, char* data, char* destination) const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const = 0;
  protected:
};

#endif

