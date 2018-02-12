#ifndef MaxSATFormulaExtended_h
#define MaxSATFormulaExtended_h

#include "MaxSATFormula.h"

namespace openwbo {

class MaxSATFormulaExtended : public MaxSATFormula {

public:
  vec<Soft>& getSoftClauses();

};

}

#endif
