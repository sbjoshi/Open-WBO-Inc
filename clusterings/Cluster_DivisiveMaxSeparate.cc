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
	cluster_indices.push({0,false});
	distances.growTo(original_weights.size()-1);
	for(uint64_t i = 0; i < original_weights.size()-1; i++) {
	  distances[i] = original_weights[i+1] - original_weights[i];
	}
}

void Cluster_DivisiveMaxSeparate::clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) {
  if(c > original_weights.size()) {
  	printf("c Limiting number of clusters to number of weights\n");
    c = original_weights.size();
  }
  if(c == max_c) {
    replaceWeights(formula, cluster_indices);
  }
  else {
    vec<cluster_index> temp;
    if(c < max_c) {
      max_c = 1;
      cluster_indices.clear();
      cluster_indices.push({0,false});      
    }
    uint64_t low_index, high_index, max_distance, max_index;
    bool intra_nonzero;
    for(uint64_t i = max_c; i < c; i++) {
      temp.clear();
      max_distance = 0;
      max_index = 0;
      for(uint64_t j = 0; j < cluster_indices.size(); j++) {
        if(cluster_indices[j].all_equal_in_cluster) {
          continue;
        }
        low_index = cluster_indices[j].index;
        high_index = (j == cluster_indices.size()-1 ? 
                      original_weights.size()-1 : cluster_indices[j+1].index-1);
        intra_nonzero = false;
        for(uint64_t k = low_index; k < high_index; k++) {
        	if(distances[k] > max_distance) {
        		max_distance = distances[k];
        		max_index = k+1;
        	}
        	if(distances[k] > 0) {
        		intra_nonzero = true;
        	}
        }
        if(!intra_nonzero) {
          cluster_indices[j].all_equal_in_cluster = true;
        }
      }
      // don't split if weights are equal anyway
      if(max_distance == 0) {
        // need to add check for new clusters with intra cluster distance 
        // zero. not necessary until update to number of clusters is enabled
        // TODO
      	printf("c Redundant split found\n");
      	break;
      } 
      bool is_added = false;
      for(uint64_t j = 0; j < cluster_indices.size(); j++) {
        if(cluster_indices[j].index > max_index && !is_added) {
          temp.push({max_index,false});
          is_added = true;
        }
        temp.push(cluster_indices[j]);
      }
      if(max_index >= cluster_indices[cluster_indices.size()-1].index) {
        temp.push({max_index,false});
      }
      temp.copyTo(cluster_indices);
    }
//    for(int i1 = 0; i1 < cluster_indices.size(); i1++) {
//      printf("%llu ",cluster_indices[i1]);
//    }
//    printf("\n");
    replaceWeights(formula,cluster_indices);
    max_c = c;
  }
}
