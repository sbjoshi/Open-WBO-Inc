#include <algorithm>
#include <iostream>
#include <vector>

#include "MaxSATFormulaExtended.h"

using namespace openwbo;

// Compares two soft clauses according to their weights
bool compare(const Soft &a, const Soft &b) { return a.weight < b.weight; }

// Sorts soft clauses accordiing to weights of the soft clauses
void MaxSATFormulaExtended::sortSoftClauses() {
  uint64_t size = soft_clauses.size();
  std::sort(soft_clauses + 0, soft_clauses + size,
            [](const Soft &a, const Soft &b) { return a.weight < b.weight; });
}

// Returns soft clauses
vec<Soft> &MaxSATFormulaExtended::getSoftClauses() { return soft_clauses; }
