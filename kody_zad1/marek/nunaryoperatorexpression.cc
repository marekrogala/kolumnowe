#include <cassert>
#include <sstream>
#include "nunaryoperatorexpression.h"


template<ColType T, typename TOp>
NUnaryOperatorExpression<T, TOp>::NUnaryOperatorExpression(const NExpression* left) : left_(left){
}


template<ColType T, typename TOp>
std::string NUnaryOperatorExpression<T, TOp>::toString() const {
  std::ostringstream ss;
  ss << "NUnaryOperatorExpression<";
  ss << T;
  ss << ">(left=";
  ss << this->left_;
  ss << ")";
  return ss.str();
}

template<ColType T, typename TOp>
NUnaryOperatorExpression<T, TOp>:: ~NUnaryOperatorExpression(){
  delete this->left_;
}

template<ColType T, typename TOp>
template<ColType L>
void NUnaryOperatorExpression<T, TOp>::leftTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const{
  typename TypeTraits<L>::T * leftData = (typename TypeTraits<L>::T *) malloc(number * sizeof(typename TypeTraits<L>::T));

  static_cast<const NTypedExpression<L> *>(this->left_)->compute(number, data, leftData);

//  for(int i = 0; i < number; ++i){
//      this->op_(leftData[i], destination[i]);
//  }

  int i=0;
  int  size = number - 8;
  for(i = 0; i < size; i+=8){
    this->op_(leftData[i], destination[i]);
    this->op_(leftData[i+1], destination[i+1]);
    this->op_(leftData[i+2], destination[i+2]);
    this->op_(leftData[i+3], destination[i+3]);
    this->op_(leftData[i+4], destination[i+4]);
    this->op_(leftData[i+5], destination[i+5]);
    this->op_(leftData[i+6], destination[i+6]);
    this->op_(leftData[i+7], destination[i+7]);
  }
  for(; i < number; ++i){
    this->op_(leftData[i], destination[i]);
  }

  free(leftData);
}

template<ColType T, typename TOp>
void NUnaryOperatorExpression<T, TOp>::compute(int number, char* data, typename TypeTraits<T>::T * destination) const {
  switch(this->left_->getType()){
    case BOOL: this->leftTypedCompute<BOOL>(number, data, destination); break;
    case INT: this->leftTypedCompute<INT>(number, data, destination); break;
    case DOUBLE: this->leftTypedCompute<DOUBLE>(number, data, destination); break;
  }
}

template class NUnaryOperatorExpression<INT, OpNEGATE>;
template class NUnaryOperatorExpression<DOUBLE, OpNEGATE>;

template class NUnaryOperatorExpression<DOUBLE, OpLOG>;

template class NUnaryOperatorExpression<BOOL, OpNOT>;
