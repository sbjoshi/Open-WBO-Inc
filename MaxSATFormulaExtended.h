#ifndef MaxSATFormulaExtended_h
#define MaxSATFormulaExtended_h

#include "MaxSATFormula.h"

namespace openwbo {

class MaxSATFormulaExtended : public MaxSATFormula {

public:
//  vec<Soft> soft_clauses;
  // vec<Soft>& getSoftClauses();
  void sortSoftClauses();
  vec<Soft> &getSoftClauses();

};

}

#endif
