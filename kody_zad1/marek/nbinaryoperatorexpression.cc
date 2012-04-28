#include <cassert>
#include <sstream>
#include "nbinaryoperatorexpression.h"


template<ColType T, typename TOp>
NBinaryOperatorExpression<T, TOp>::NBinaryOperatorExpression(const NExpression* left,
      const NExpression* right) : left_(left), right_(right){
}


template<ColType T, typename TOp>
std::string NBinaryOperatorExpression<T, TOp>::toString() const {
  std::ostringstream ss;
  ss << "NBinaryOperatorExpression<";
  ss << T;
  ss << ">(left=";
  ss << this->left_;
  ss << ", right=";
  ss << this->right_;
  ss << ")";
  return ss.str();
}

template<ColType T, typename TOp>
NBinaryOperatorExpression<T, TOp>:: ~NBinaryOperatorExpression(){
  delete this->left_;
  delete this->right_;
}

template<ColType T, typename TOp>
template<ColType L, ColType R>
void NBinaryOperatorExpression<T, TOp>::fullyTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const{
  typename TypeTraits<L>::T * leftData = (typename TypeTraits<L>::T *) malloc(number * sizeof(typename TypeTraits<L>::T));
  typename TypeTraits<R>::T * rightData = (typename TypeTraits<R>::T *) malloc(number * sizeof(typename TypeTraits<R>::T));

  static_cast<const NTypedExpression<L> *>(this->left_)->compute(number, data, leftData);
  static_cast<const NTypedExpression<R> *>(this->right_)->compute(number, data, rightData);


//  for(int i = 0; i < number; ++i){
//      this->op_(leftData[i], rightData[i], destination[i]);
//  }

  int i=0;
  int  size = number - 8;
  for(i = 0; i < size; i+=8){
    this->op_(leftData[i], rightData[i], destination[i]);
    this->op_(leftData[i+1], rightData[i+1], destination[i+1]);
    this->op_(leftData[i+2], rightData[i+2], destination[i+2]);
    this->op_(leftData[i+3], rightData[i+3], destination[i+3]);
    this->op_(leftData[i+4], rightData[i+4], destination[i+4]);
    this->op_(leftData[i+5], rightData[i+5], destination[i+5]);
    this->op_(leftData[i+6], rightData[i+6], destination[i+6]);
    this->op_(leftData[i+7], rightData[i+7], destination[i+7]);
  }
  for(; i < number; ++i){
      this->op_(leftData[i], rightData[i], destination[i]);
  }

  free(leftData);
  free(rightData);
}

template<ColType T, typename TOp>
template<ColType L>
void NBinaryOperatorExpression<T, TOp>::leftTypedCompute(int number, char* data, typename TypeTraits<T>::T * destination) const {
  switch(this->right_->getType()){
    case BOOL: this->fullyTypedCompute<L, BOOL>(number, data, destination); break;
    case INT: this->fullyTypedCompute<L, INT>(number, data, destination); break;
    case DOUBLE: this->fullyTypedCompute<L, DOUBLE>(number, data, destination); break;
  }
}

template<ColType T, typename TOp>
void NBinaryOperatorExpression<T, TOp>::compute(int number, char* data, typename TypeTraits<T>::T * destination) const {
  switch(this->left_->getType()){
    case BOOL: this->leftTypedCompute<BOOL>(number, data, destination); break;
    case INT: this->leftTypedCompute<INT>(number, data, destination); break;
    case DOUBLE: this->leftTypedCompute<DOUBLE>(number, data, destination); break;
  }
}

template class NBinaryOperatorExpression<INT, OpADD>;
template class NBinaryOperatorExpression<DOUBLE, OpADD>;

template class NBinaryOperatorExpression<INT, OpSUBTRACT>;
template class NBinaryOperatorExpression<DOUBLE, OpSUBTRACT>;

template class NBinaryOperatorExpression<INT, OpMULTIPLY>;
template class NBinaryOperatorExpression<DOUBLE, OpMULTIPLY>;

template class NBinaryOperatorExpression<DOUBLE, OpFLOATING_DIVIDE>;

template class NBinaryOperatorExpression<BOOL, OpLOWER>;
template class NBinaryOperatorExpression<BOOL, OpGREATER>;
template class NBinaryOperatorExpression<BOOL, OpEQUAL>;
template class NBinaryOperatorExpression<BOOL, OpNOT_EQUAL>;
template class NBinaryOperatorExpression<BOOL, OpAND>;
template class NBinaryOperatorExpression<BOOL, OpOR>;


