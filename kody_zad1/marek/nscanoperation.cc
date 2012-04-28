#include <cassert>
#include <sstream>
#include <iostream>
#include "nscanoperation.h"

NScanOperation::NScanOperation(const ScanOperation &op){
  for(int i = 0, size = op.column().size(); i < size; ++i){
    ScanOperation_Type rawType = op.type(i);
    ColType type = (rawType == ScanOperation_Type_INT) ? INT : ((rawType == ScanOperation_Type_BOOL) ? BOOL : DOUBLE);
    this->columns_.push_back(std::make_pair(op.column(i), type));
  }
  this->calculateOffsets();
}

std::string NScanOperation::toString() const {
  std::ostringstream ss;
  ss << "NScanOperation( ";
  for(int i = 0, size = this->columns_.size(); i < size; ++i){
    ss << this->columns_[i].first;
    ss << ":";
    ss << this->columns_[i].second;
    ss << " ";
  }
  ss << ")";
  return ss.str();
}

ColType NScanOperation::getColumnType(int column_id) const {
  return this->columns_[column_id].second;
}

int NScanOperation::getColumnCount() const {
  return this->columns_.size();
}

int NScanOperation::getColumnOriginalNumber(int column_id){
  return this->columns_[column_id].first;
}

int NScanOperation::scanColumn(int column_id, int number, char* ptr){
  switch(this->getColumnType(column_id)){
    case BOOL:
      return this->server_->GetByteBools(this->getColumnOriginalNumber(column_id), number, (TypeTraits<BOOL>::T *)ptr);
    case INT:
      return this->server_->GetInts(this->getColumnOriginalNumber(column_id), number, (TypeTraits<INT>::T*)ptr);
    case DOUBLE:
      return this->server_->GetDoubles(this->getColumnOriginalNumber(column_id), number, (TypeTraits<DOUBLE>::T*)ptr);
    default:
      assert(false);
      return 0;
  }
}

int NScanOperation::fetch(int number, char* &destination){
  char* ptr = this->allocateBlock();
  int available = this->scanColumn(0, number, ptr);

  if(available == 0){
    free(ptr);
    destination = NULL;
  } else {
    for(int column_id = 1, size = this->getColumnCount(); column_id < size; ++column_id){
        char* col_ptr = this->calculateColumnPosition(ptr, column_id);
        this->scanColumn(column_id, available, col_ptr);
    }
    destination = ptr;
  }

  return available;
}
