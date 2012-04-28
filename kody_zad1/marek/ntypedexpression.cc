#include "ntypedexpression.h"

#include <sstream>

template<ColType T>
std::string NTypedExpression<T>::toString() const {
  std::ostringstream ss;
  ss << "NTypedExpression";
  return ss.str();
}

template<ColType T>
ColType NTypedExpression<T>::getType() const {
  return T;
}

template<ColType T>
void NTypedExpression<T>::untypedCompute(int number, char* data, char* destination) const {
  this->compute(number, data, (typename TypeTraits<T>::T *)destination);
}

template class NTypedExpression<INT>;
template class NTypedExpression<DOUBLE>;
template class NTypedExpression<BOOL>;

