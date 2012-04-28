#ifndef NBINARYOPERATOREXPRESSION_H_
#define NBINARYOPERATOREXPRESSION_H_

#include "proto/operations.pb.h"
#include "ntypedexpression.h"
#include "BinaryOperators.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>


template<ColType T, typename TOp>
class NBinaryOperatorExpression : public NTypedExpression<T> {
  public:
    virtual ~NBinaryOperatorExpression();
    NBinaryOperatorExpression(const NExpression* left, const NExpression* right);
    virtual std::string toString() const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const;
  private:
    TOp op_;
    const NExpression* left_;
    const NExpression* right_;
    template<ColType L, ColType R> void fullyTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const;
    template<ColType L> void leftTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const;

};

#endif
