#ifndef NEXPRESSION_H_
#define NEXPRESSION_H_

#include "proto/operations.pb.h"
#include "typetraits.h"
#include "noperation.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

class NExpression {
  public:
    virtual ~NExpression() {}
    static NExpression* FromProtobuf(const Expression &op, const NOperation * source);
    virtual std::string toString() const;
    virtual ColType getType() const = 0;
    virtual void untypedCompute(int number, char* data, char* destination) const = 0;
  protected:
    NExpression(){}

};

std::ostream& operator<<(std::ostream& out, const NExpression * const nop);

#endif

