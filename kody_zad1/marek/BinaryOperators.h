#ifndef BINARYOPERATORS_H
#define BINARYOPERATORS_H

#include <iostream>

struct OpADD {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = ((TTR)left) + right;
  }
};

struct OpSUBTRACT {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = ((TTR)left) - right;
  }
};

struct OpMULTIPLY {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = ((TTR)left) * right;
  }
};

struct OpFLOATING_DIVIDE {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = ((TTR)left) / right;
  }
};

struct OpLOWER {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left < right;
  }
};
struct OpGREATER {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left > right;
  }
};
struct OpEQUAL {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left == right;
  }
};
struct OpNOT_EQUAL {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left != right;
  }
};
struct OpAND {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left && right;
  }
};
struct OpOR {
  template<typename TT1, typename TT2, typename TTR>
  void operator()(const TT1 &left, const TT2 &right, TTR &result) const {
    result = left || right;
  }
};

#endif
