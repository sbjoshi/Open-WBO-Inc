#ifndef MaxSATFormulaExtended_h
#define MaxSATFormulaExtended_h

#include "MaxSATFormula.h"

namespace openwbo {

class MaxSATFormulaExtended : public MaxSATFormula {

public:
  void sortSoftClauses();
  vec<Soft> &getSoftClauses();
};

} // namespace openwbo

#endif
