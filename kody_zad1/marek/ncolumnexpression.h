#ifndef NCOLUMNEXPRESSION_H_
#define NCOLUMNEXPRESSION_H_

#include "proto/operations.pb.h"
#include "ntypedexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>


template<ColType T>
class NColumnExpression : public NTypedExpression<T> {
  public:
    virtual ~NColumnExpression() {}
    NColumnExpression(const Expression &op, const NOperation* source);
    virtual std::string toString() const;
    virtual void compute(int number, char* data, typename TypeTraits<T>::T * destination) const;
  private:
    int column_id_;
    const NOperation* source_;
};

#endif
