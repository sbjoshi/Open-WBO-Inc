#include "Cluster_DivisiveMaxSeparate.h"

using namespace openwbo;

Cluster_DivisiveMaxSeparate::Cluster_DivisiveMaxSeparate(
	MaxSATFormulaExtended *formula, Statistics cluster_statistic
) : Cluster(formula, cluster_statistic) {
	max_c = 1;
	vec<uint64_t> temp;
	temp.push(1);
	cluster_indices.push(temp);
	distances.growTo(original_weights.size()-1);
	for(uint64_t i = 0; i < original_weights.size()-1; i++) {
	  distances[i] = original_weights[i+1] - original_weights[i];
	}
}

void Cluster_DivisiveMaxSeparate::clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) {
	if(max_c >= c) {
	  replaceWeights(formula,cluster_indices[c]);
	}
	else {
	  if(c > original_weights.size()) {
	    c = original_weights.size();
	  }	
	  vec<uint64_t> temp;
	  uint64_t previous_index = max_c;
	  uint64_t low_index, high_index, max_distance, max_index;
	  for(uint64_t i = max_c+1; i <= c; i++) {
	    temp.clear();
	    max_distance = 0;
	    max_index = 0;
	    for(uint64_t j = 0; j < cluster_indices[previous_index].size(); j++) {
	      low_index = cluster_indices[previous_index][j];
	      high_index = (j == cluster_indices[previous_index].size()-1 ? distances.size()-1 : cluster_indices[previous_index][j+1]-1);
	      for(uint64_t k = low_index; k < high_index; k++) {
	      	if(distances[k] >= max_distance) {
	      		max_distance = distances[k];
	      		max_index = k;
	      	}
	      }
	    }
	    for(uint64_t j = 0; j < cluster_indices[previous_index].size(); j++) {
	      if(cluster_indices[previous_index][j] > max_index) {
	        temp.push(max_index);
	      }
	      else {
	        temp.push(cluster_indices[previous_index][j]);
	      }
	    }
	    cluster_indices.push(temp);
	    previous_index = i;
	  }
	  replaceWeights(formula,cluster_indices[c]);
	}
}
