#include "Cluster.h"

#include <cstdio>

using namespace openwbo;

Cluster::Cluster(MaxSATFormulaExtended *formula, Statistics cluster_stat) {
  assert(formula != NULL);
  formula->sortSoftClauses();
  saveWeights(formula);
  cluster_statistic = cluster_stat; // is this needed?
  statistic_finder.setStatistic(cluster_statistic);
  //vec<Soft> soft_clauses = formula->soft_clauses;
  num_clauses = formula->getSoftClauses().size();
}

void Cluster::saveWeights(MaxSATFormulaExtended *formula) {
  if(formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  original_weights.growTo(soft_clauses.size());
  for(int i = 0; i < soft_clauses.size(); i++) {
 // 	printf("1 %d %d\n",i,original_weights[i]);
 // 	printf("2 %d %d\n",i,formula->soft_clauses[i].weight);
    original_weights[i] = soft_clauses[i].weight;
  }
  printf("SAVED ORIGINAL WEIGHTS!!!\n");
}

void Cluster::restoreWeights(MaxSATFormulaExtended *formula) {
  if(formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  assert(original_weights.size() != soft_clauses.size());
  for(int i = 0; i < soft_clauses.size(); i++) {
    soft_clauses[i].weight = original_weights[i]; // IMPORTANT - soft_clauses 
    // must be received by reference. Needs check
  }
}

void Cluster::replaceWeights(MaxSATFormulaExtended *formula, vec<uint64_t> &clusters) {
	if(formula == NULL) {
	  return;
	}
	vec<Soft> &soft_clauses = formula->getSoftClauses();
	uint64_t low_index, high_index, replacement_weight;
	for(uint64_t i = 0; i < clusters.size(); i++) {
	  low_index = clusters[i];
	  high_index = (i == clusters.size() -1 ? original_weights.size() -1 : clusters[i+1]-1);
	  replacement_weight = statistic_finder.getSequenceStatistic(original_weights,low_index,high_index);
	  for(uint64_t j = low_index; j <= high_index; j++) {
	    soft_clauses[j].weight = replacement_weight;
	  }  
	}
}

uint64_t Cluster::getOriginalWeight(int index) {
	assert(index >= 0 && index < original_weights.size());
	return original_weights[index];
}
