#ifndef NCOMPUTEOPERATION_H_
#define NCOMPUTEOPERATION_H_

#include "proto/operations.pb.h"
#include "noperation.h"
#include "nexpression.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

class NComputeOperation : public NOperation {
  public:
    virtual ~NComputeOperation();
    NComputeOperation(const ComputeOperation &op);
    virtual std::string toString() const;
    virtual ColType getColumnType(int column_id) const;
    virtual int getColumnCount() const;
    virtual void setServer(class Server * server);
    virtual int fetch(int number, char* &destination);
  private:
    NOperation * source_;
    std::vector<NExpression*> expressions_;
};

#endif

