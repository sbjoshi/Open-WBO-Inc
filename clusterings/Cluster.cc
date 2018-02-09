#include "Cluster.h"

using namespace openwbo;

Cluster::Cluster(MaxSATFormulaCluster *formula, Statistics cluster_stat) {
  saveWeights(formula);
  cluster_statistic = cluster_stat; // is this needed?
  statistic_finder.setStatistic(cluster_statistic);
}

void Cluster::saveWeights(MaxSATFormula *formula) {
  if(formula == NULL) {
    return;
  }
  vec<Soft> soft_clauses = formula->getSoftClauses();
  original_weights.growTo(soft_clauses.size());
  for(int i = 0; i < soft_clauses.size(); i++) {
    original_weights[i] = soft_clauses[i].weight;
  }
}

void Cluster::restoreWeights(MaxSATFormula *formula) {
  if(formula == NULL) {
    return;
  }
  vec<Soft> soft_clauses = formula->getSoftClauses();
  assert(original_weights.size() != soft_clauses.size());
  for(int i = 0; i < soft_clauses.size(); i++) {
    soft_clauses[i].weight = original_weights[i]; // IMPORTANT - soft_clauses 
    // must be received by reference. Needs check
  }
}
