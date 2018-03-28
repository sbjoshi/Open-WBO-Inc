#include "Cluster_DivisiveMaxSeparate.h"

using namespace openwbo;

template <typename T>
vec<T>& convertVectorToVec(std::vector<T> inputs) {
	vec<T> result(inputs.size());
	for(int i = 0; i < inputs.size(); i++) {
		result[i] = inputs[i];
	}
	return result;
}

Cluster_DivisiveMaxSeparate::Cluster_DivisiveMaxSeparate(
	MaxSATFormulaExtended *formula, Statistics cluster_statistic
) : Cluster(formula, cluster_statistic) {
	max_c = 1;
	cluster_indices.push(0);
	printf("CI SIZE : %d\n",cluster_indices.size());
	distances.growTo(original_weights.size()-1);
	for(uint64_t i = 0; i < original_weights.size()-1; i++) {
	  distances[i] = original_weights[i+1] - original_weights[i];
	}
}

void Cluster_DivisiveMaxSeparate::clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) {
  if(c > original_weights.size()) {
    c = original_weights.size();
  }
  if(c == max_c) {
    replaceWeights(formula, cluster_indices);
  }
  else {
    vec<uint64_t> temp;
    if(c < max_c) {
      max_c = 1;
      cluster_indices.clear();
      cluster_indices.push(0);
    }
    uint64_t low_index, high_index, max_distance, max_index;
    for(uint64_t i = max_c; i < c; i++) {
      temp.clear();
      max_distance = 0;
      max_index = 0;
      for(uint64_t j = 0; j < cluster_indices.size(); j++) {
        low_index = cluster_indices[j];
        high_index = (j == cluster_indices.size()-1 ? original_weights.size()-1 : cluster_indices[j+1]-1);
        printf("LI : %d HI : %d\n",low_index,high_index);
        for(uint64_t k = low_index; k < high_index; k++) {
        	if(distances[k] >= max_distance) {
        		printf("FOUND K : %d, DISTANCES[k] : %d, Max distance : %d\n",k,distances[k],max_distance);
        		max_distance = distances[k];
        		max_index = k+1; // TODO BE VERY CAREFUL FOR CHECKING LAST ELEMENT! TODO CHECK!
        	}
        }
      }
      // don't split if weights are equal anyway
      if(max_distance == 0) {
      	printf("Redundant split found\n");
      	break;
      } 
      bool is_added = false;
      for(uint64_t j = 0; j < cluster_indices.size(); j++) {
        if(cluster_indices[j] > max_index && !is_added) {
          temp.push(max_index);
          is_added = true;
        }
      temp.push(cluster_indices[j]);
      }
      if(max_index >= cluster_indices[cluster_indices.size()-1]) {
        temp.push(max_index);
      }
      temp.copyTo(cluster_indices);
    }
    for(int i1 = 0; i1 < cluster_indices.size(); i1++) {
      printf("%d ",cluster_indices[i1]);
    }
    printf("\n");
    replaceWeights(formula,cluster_indices);
  }
}
