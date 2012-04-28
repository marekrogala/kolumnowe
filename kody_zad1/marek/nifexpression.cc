#include <cassert>
#include <sstream>
#include "nifexpression.h"

template<ColType T>
NIfExpression<T>::NIfExpression(NExpression * condition,
		NExpression * optionTrue,
		NExpression * optionFalse){
	  this->condition_ = dynamic_cast<NTypedExpression<BOOL> *> (condition);
	  this->optionTrue_ = dynamic_cast<NTypedExpression<T> *> (optionTrue);
	  this->optionFalse_ = dynamic_cast<NTypedExpression<T> *> (optionFalse);

	  assert(this->condition_);
	  assert(this->optionTrue_);
	  assert(this->optionFalse_);
}

template<ColType T>
std::string NIfExpression<T>::toString() const {
  std::ostringstream ss;
  ss << "NIfExpression<";
  ss << T;
  ss << ">(condition=";
  ss << this->condition_;
  ss << ", true=";
  ss << this->optionTrue_;
  ss << ", false=";
  ss << this->optionFalse_;
  ss << ")";
  return ss.str();
}

template<ColType T>
NIfExpression<T>:: ~NIfExpression(){
  delete this->condition_;
  delete this->optionTrue_;
  delete this->optionFalse_;
}

template<ColType T>
void NIfExpression<T>::compute(int number, char* data, typename TypeTraits<T>::T * destination) const {
  typename TypeTraits<BOOL>::T * conditionData = (typename TypeTraits<BOOL>::T *) malloc(number * sizeof(typename TypeTraits<BOOL>::T));
  typename TypeTraits<T>::T * leftData = (typename TypeTraits<T>::T *) malloc(number * sizeof(typename TypeTraits<T>::T));
  typename TypeTraits<T>::T * rightData = (typename TypeTraits<T>::T *) malloc(number * sizeof(typename TypeTraits<T>::T));

  static_cast<const NTypedExpression<BOOL> *>(this->condition_)->compute(number, data, conditionData);
  static_cast<const NTypedExpression<T> *>(this->optionTrue_)->compute(number, data, leftData);
  static_cast<const NTypedExpression<T> *>(this->optionFalse_)->compute(number, data, rightData);

//  for(int i = 0; i < number; ++i){
//    destination[i] = conditionData[i] ? leftData[i] : rightData[i];
//  }

  int i=0;
  int  size = number - 8;
  for(i = 0; i < size; i+=8){
    destination[i] = conditionData[i] ? leftData[i] : rightData[i];
    destination[i+1] = conditionData[i+1] ? leftData[i+1] : rightData[i+1];
    destination[i+2] = conditionData[i+2] ? leftData[i+2] : rightData[i+2];
    destination[i+3] = conditionData[i+3] ? leftData[i+3] : rightData[i+3];
    destination[i+4] = conditionData[i+4] ? leftData[i+4] : rightData[i+4];
    destination[i+5] = conditionData[i+5] ? leftData[i+5] : rightData[i+5];
    destination[i+6] = conditionData[i+6] ? leftData[i+6] : rightData[i+6];
    destination[i+7] = conditionData[i+7] ? leftData[i+7] : rightData[i+7];
  }
  for(; i < number; ++i){
    destination[i] = conditionData[i] ? leftData[i] : rightData[i];
  }


  free(conditionData);
  free(leftData);
  free(rightData);
}

template class NIfExpression<INT>;
template class NIfExpression<DOUBLE>;
template class NIfExpression<BOOL>;

