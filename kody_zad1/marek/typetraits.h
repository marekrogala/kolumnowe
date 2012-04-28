#ifndef TYPETRAITS_H_
#define TYPETRAITS_H_

enum ColType {
  BOOL = 0,
  INT = 1,
  DOUBLE = 2
};

const int SizeOfType[] = {sizeof(bool), sizeof(int), sizeof(double)};

template<ColType Type>
struct TypeTraits  {
};

template <>
struct TypeTraits<BOOL>{
  typedef bool T;
};

template <>
struct TypeTraits<INT>{
  typedef int T;
};

template <>
struct TypeTraits<DOUBLE>{
  typedef double T;
};

const int BLOCK_SIZE = 1024;

#endif

