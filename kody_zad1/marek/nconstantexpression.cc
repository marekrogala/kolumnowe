#include <cassert>
#include <sstream>
#include "nconstantexpression.h"

template<ColType T>
NConstantExpression<T>::NConstantExpression(typename TypeTraits<T>::T constant_value){
  this->constant_value_ = constant_value;
}

template<ColType T>
std::string NConstantExpression<T>::toString() const {
  std::ostringstream ss;
  ss << "NConstantExpression<";
  ss << T;
  ss << ">(value=";
  ss << this->constant_value_;
  ss << ")";
  return ss.str();
}

template<ColType T>
void NConstantExpression<T>::compute(int number, char* data, typename TypeTraits<T>::T * destination) const {
//  for(int i = 0; i < number; ++i){
//    destination[i] = this->constant_value_;
//  }
  int i=0;
  int  size = number - 8;
  for(; i < size; i+=8){
    destination[i] = this->constant_value_;
    destination[i+1] = this->constant_value_;
    destination[i+2] = this->constant_value_;
    destination[i+3] = this->constant_value_;
    destination[i+4] = this->constant_value_;
    destination[i+5] = this->constant_value_;
    destination[i+6] = this->constant_value_;
    destination[i+7] = this->constant_value_;
  }
  for(; i < number; ++i){
    destination[i] = this->constant_value_;
  }
}

template class NConstantExpression<INT>;
template class NConstantExpression<DOUBLE>;
template class NConstantExpression<BOOL>;

