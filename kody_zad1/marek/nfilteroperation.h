#ifndef NFILTEROPERATION_H_
#define NFITLEROPERATION_H_

#include "proto/operations.pb.h"
#include "noperation.h"
#include "nexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

class NFilterOperation : public NOperation {
  public:
    virtual ~NFilterOperation();
    NFilterOperation(const FilterOperation &op);
    virtual std::string toString() const;
    virtual ColType getColumnType(int column_id) const;
    virtual int getColumnCount() const;
    virtual void setServer(class Server * server);
    virtual int fetch(int number, char* &destination);
  private:
    NOperation * source_;
    NExpression* expression_;
};

#endif

