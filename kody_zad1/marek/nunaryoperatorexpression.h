#ifndef NUNARYOPERATOREXPRESSION_H_
#define NUNARYOPERATOREXPRESSION_H_

#include "proto/operations.pb.h"
#include "ntypedexpression.h"
#include "UnaryOperators.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>


template<ColType T, typename TOp>
class NUnaryOperatorExpression : public NTypedExpression<T> {
  public:
    virtual ~NUnaryOperatorExpression();
    NUnaryOperatorExpression(const NExpression* left);
    virtual std::string toString() const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const;
  private:
    const NExpression* left_;
    TOp op_;
    template<ColType L> void leftTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const;
};

#endif
