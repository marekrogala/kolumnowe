#include <cassert>
#include <sstream>
#include <iostream>
#include "nfilteroperation.h"
#include "noperation.h"
#include "nexpression.h"
#include "ntypedexpression.h"

NFilterOperation::NFilterOperation(const FilterOperation &op){
  this->source_ = NOperation::FromProtobuf(op.source());
  this->expression_ = NExpression::FromProtobuf(op.expression(), this->source_);
  this->calculateOffsets();
}

std::string NFilterOperation::toString() const {
  std::ostringstream ss;
  ss << "NFilterOperation( source=";
  ss << this->source_;
  ss << ", expression=";
  ss << this->expression_;
  ss << " )";
  return ss.str();
}

NFilterOperation::~NFilterOperation(){
  delete this->source_;
  delete this->expression_;
}

ColType NFilterOperation::getColumnType(int column_id) const {
  return this->source_->getColumnType(column_id);
}

int NFilterOperation::getColumnCount() const {
  return this->source_->getColumnCount();
}

void NFilterOperation::setServer(class Server * server)
{
  NOperation::setServer(server);
  this->source_->setServer(server);
}

template<ColType T>
int RewriteColumn(int newIndex, int source_number, char* col_raw_result_ptr, char* col_raw_source_ptr, typename TypeTraits<BOOL>::T * condition_data){
  typename TypeTraits<T>::T * typed_dest = ((typename TypeTraits<T>::T *)col_raw_result_ptr);
  typename TypeTraits<T>::T * typed_source = ((typename TypeTraits<T>::T *) col_raw_source_ptr);
//  for(int i = 0; i < source_number; i++){
//      if(condition_data[i]){
//          typed_dest[newIndex++] = typed_source[i];
//      }
//  }
  //CONTROL DEPENDENCY --> DATA DEPENDENCY
  for(int i = 0; i < source_number; i++){
    typed_dest[newIndex] = typed_source[i];
    newIndex += condition_data[i]?1:0;
  }
  return newIndex;
}

int NFilterOperation::fetch(int number, char* &destination){
  char* source_data;
  int source_number;
  destination = this->allocateBlock();

  int passed = 0, newIndex = 0;
  TypeTraits<BOOL>::T * condition_data = (TypeTraits<BOOL>::T *) malloc(number * sizeof(TypeTraits<BOOL>::T));

  while(passed*2 < number){
      source_number = this->source_->fetch(number - passed, source_data);
      if(source_number == 0) break;

      //1. calculate condition expression value
      static_cast<NTypedExpression<BOOL> *>(this->expression_)->compute(source_number, source_data, condition_data);

      //2. rewrite each column
      for(int col = 0, size = this->getColumnCount(); col < size; ++col){
          char* col_raw_result_ptr = this->calculateColumnPosition(destination, col);
          char* col_raw_source_ptr = this->calculateColumnPosition(source_data, col);

        switch(this->getColumnType(col)){
          case BOOL:
            newIndex = RewriteColumn<BOOL>(passed, source_number, col_raw_result_ptr, col_raw_source_ptr, condition_data);
            break;
          case INT:
            newIndex = RewriteColumn<INT>(passed, source_number, col_raw_result_ptr, col_raw_source_ptr, condition_data);
            break;
          case DOUBLE:
            newIndex = RewriteColumn<DOUBLE>(passed, source_number, col_raw_result_ptr, col_raw_source_ptr, condition_data);
            break;
        }
      }

      passed = newIndex;
      free(source_data);
  }

  free(condition_data);

  if(passed == 0){
      free(destination);
  }
  return passed;
}
