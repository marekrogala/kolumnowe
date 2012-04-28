#include <cassert>
#include <sstream>
#include <iostream>
#include "ncolumnexpression.h"


template<ColType T>
NColumnExpression<T>::NColumnExpression(const Expression &op, const NOperation* source):
  source_(source)
{
  assert(op.operator_() == Expression_Operator_COLUMN);
  this->column_id_ = op.column_id();
}

template<ColType T>
std::string NColumnExpression<T>::toString() const {
  std::ostringstream ss;
  ss << "NColumnExpression<";
  ss << T;
  ss << ">(column_id=";
  ss << this->column_id_;
  ss << ")";
  return ss.str();
}

template<ColType T>
void NColumnExpression<T>::compute(int number, char* data, typename TypeTraits<T>::T * destination) const {
  typename TypeTraits<T>::T * source_ptr = (typename TypeTraits<T>::T *) this->source_->calculateColumnPosition(data, this->column_id_);
  memcpy(destination, source_ptr, number * sizeof(typename TypeTraits<T>::T));
}

template class NColumnExpression<INT>;
template class NColumnExpression<DOUBLE>;
template class NColumnExpression<BOOL>;
