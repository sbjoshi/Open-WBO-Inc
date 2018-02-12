#include <iostream>

#include "MaxSATFormulaExtended.h"

using namespace openwbo;

vec<Soft>& MaxSATFormulaExtended::getSoftClauses() {
	return soft_clauses;
}