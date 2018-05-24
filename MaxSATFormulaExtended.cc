#include <algorithm>
#include <iostream>
#include <vector>

#include "MaxSATFormulaExtended.h"

using namespace openwbo;

bool compare(const Soft &a, const Soft &b) { return a.weight < b.weight; }

void MaxSATFormulaExtended::sortSoftClauses() {
  uint64_t size = soft_clauses.size();
  std::sort(soft_clauses + 0, soft_clauses + size,
            [](const Soft &a, const Soft &b) { return a.weight < b.weight; });
}

vec<Soft> &MaxSATFormulaExtended::getSoftClauses() { return soft_clauses; }
