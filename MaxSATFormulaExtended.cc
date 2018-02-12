#include <iostream>
#include <algorithm>

#include "MaxSATFormulaExtended.h"

using namespace openwbo;

vec<Soft>& MaxSATFormulaExtended::getSoftClauses() {
	return soft_clauses;
}

void MaxSATFormulaExtended::sortSoftClauses() {
	std::sort(*soft_clauses, *soft_clauses + soft_clauses.size(), [](Soft a, Soft b){
		return a.weight <= b.weight;
	});
}