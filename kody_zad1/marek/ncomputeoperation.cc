#include <cassert>
#include <sstream>
#include <iostream>
#include "ncomputeoperation.h"
#include "noperation.h"
#include "nexpression.h"

NComputeOperation::NComputeOperation(const ComputeOperation &op){

  this->source_ = NOperation::FromProtobuf(op.source());

  for(int i = 0, size = op.expressions().size(); i < size; ++i){
    this->expressions_.push_back(NExpression::FromProtobuf(op.expressions(i), this->source_));
  }

  this->calculateOffsets();
}

std::string NComputeOperation::toString() const {
  std::ostringstream ss;
  ss << "NComputeOperation( source=";
  ss << this->source_;
  ss << ", expressions=(";
  for(int i=0, size = this->expressions_.size(); i < size; ++i){
    ss << " " << this->expressions_[i];
  }
  ss << " )";
  return ss.str();
}

NComputeOperation::~NComputeOperation(){
  delete this->source_;
  for(int i=0, size = this->expressions_.size(); i < size; ++i){
    delete this->expressions_[i];
  }
}

ColType NComputeOperation::getColumnType(int column_id) const {
  return this->expressions_[column_id]->getType();
}


int NComputeOperation::getColumnCount() const {
  return this->expressions_.size();
}

void NComputeOperation::setServer(class Server * server)
{
  NOperation::setServer(server);
  this->source_->setServer(server);
}

int NComputeOperation::fetch(int number, char* &destination){
  char* source_data;
  int source_number = this->source_->fetch(number, source_data);
  if(source_number == 0) return 0;

  destination = this->allocateBlock();

  for(int i=0, size = this->expressions_.size(); i < size; ++i){
    char* col_result_ptr = this->calculateColumnPosition(destination, i);
    this->expressions_[i]->untypedCompute(source_number, source_data, col_result_ptr);
  }

  free(source_data);
  return source_number;
}
