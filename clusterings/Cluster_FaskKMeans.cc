#include "Cluster_FastKMeans.h"

#include <cstdlib>

using namespace openwbo;

Cluster_FastKMeans::Cluster_FastKMeans(MaxSATFormulaExtended *formula,
    Statistics cluster_statistic, Statistics cost_statistic) // add a cost statistic here and change cluster statistic to it
      : Cluster(formula,cluster_statistic) {
  max_c = 1;
  d_matrix.growTo(max_c);
  t_matrix.growTo(max_c);
  d_matrix[0].growTo(num_clauses);
  t_matrix[0].growTo(num_clauses);
  uint64_t full_cost = cost(original_weights,0,num_clauses-1);
  for(uint64_t i = 0; i < num_clauses; i++) {
    d_matrix[0][i] = full_cost;
  }
  cost_statistic_finder.setStatistic(cost_statistic);
}

uint64_t Cluster_FastKMeans::cost(uint64_t start_index, uint64_t end_index) {
  // Check if there is a better way to do this
  uint64_t centroid = 
  cost_statistic_finder.getSequenceStatistic(original_weights,start_index,end_index);
  uint64_t absolute_cost = 0;
  for(uint64_t i = start_index; i <= end_index; i++) {
    absolute_cost += abs(original_weights[i] - centroid);
  }
  return absolute_cost;
}

void Cluster_FastKMeans::clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) {
  if(c > max_c) {
    d_matrix.growTo(c);
    t_matrix.growTo(c);
    for(uint64_t i = max_c; i < c; i++) {
      d_matrix[i].growTo(num_clauses); // is this legal for Glucose?
      t_matrix[i].growTo(num_clauses);
      uint64_t current_min = 0;
      for(uint64_t j = i; j < num_clauses; j++) {
        for(uint64_t k = )
      }
    }
    
  }
}
