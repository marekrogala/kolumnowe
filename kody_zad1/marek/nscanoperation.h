#ifndef SCAN_OPERATION_H_
#define SCAN_OPERATION_H_

#include "proto/operations.pb.h"
#include "noperation.h"

#include <cstring>
#include <ostream>
#include <utility>
#include <vector>

class NScanOperation : public NOperation {
  public:
    virtual ~NScanOperation() {}
    NScanOperation(const ScanOperation &op);
    virtual std::string toString() const;
    virtual ColType getColumnType(int column_id) const;
    virtual int getColumnCount() const;
    virtual int fetch(int number, char* &destination);
  private:
    std::vector<std::pair<int, ColType> > columns_;
    int scanColumn(int column_id, int number, char* ptr);
    int getColumnOriginalNumber(int column_id);
};

#endif

