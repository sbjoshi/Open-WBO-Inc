#ifndef MaxSATFormulaExtended_h
#define MaxSATFormulaExtended_h

#include "MaxSATFormula.h"

namespace openwbo {

class MaxSATFormulaExtended : public MaxSATFormula {

public:
  void sortSoftClauses(); // Sort soft clauses as per their corresponding weights
  vec<Soft> &getSoftClauses(); // Return soft clauses
};

} // namespace openwbo

#endif
