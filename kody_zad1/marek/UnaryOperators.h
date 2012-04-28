#ifndef UNARYOPERATORS_H
#define UNARYOPERATORS_H

#include <cmath>

struct OpNEGATE {
  template<typename TT1, typename TTR>
  void operator()(const TT1 &left, TTR &result) const {
    result = -left;
  }
};

struct OpLOG {
  template<typename TT1, typename TTR>
  void operator()(const TT1 &left, TTR &result) const {
    result = log(left);
  }
};

struct OpNOT {
  template<typename TT1, typename TTR>
  void operator()(const TT1 &left, TTR &result) const {
    result = !left;
  }
};

#endif
