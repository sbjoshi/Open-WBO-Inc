#include <iostream>
#include <algorithm>
#include <vector>

#include "MaxSATFormulaExtended.h"

using namespace openwbo;

// vec<Soft>& MaxSATFormulaExtended::getSoftClauses() {
// 	return soft_clauses;
// }

// bool compare(Soft &a, Soft &b) { return a.weight <= b.weight; }

void MaxSATFormulaExtended::sortSoftClauses() {
	uint64_t size = soft_clauses.size();
	/*std::vector<Soft> temp(size);
	soft_clauses[0];
	for(uint64_t i = 0; i < size; i++) {
		temp[i].copy(soft_clauses[i]);
	} 
	std::sort(temp.begin(), temp.end(), compare);
	for(uint64_t i = 0; i < size; i++) {
		soft_clauses[i].copy(temp[i]);
	}*/
	for (uint64_t i=0; i<size; i++) {
		for (uint64_t j=0; j<size-1; j++) {
			if (soft_clauses[j].weight > soft_clauses[j+1].weight) {
				Soft t;
				t.copy(soft_clauses[j]);
				soft_clauses[j].copy(soft_clauses[j+1]);
				soft_clauses[j+1].copy(t);
			}
		}
	}
}


