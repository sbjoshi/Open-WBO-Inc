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
	vector<uint64_t> temp;
	temp.push_back(0);
	cluster_indices.push_back(temp);
	printf("CI SIZE : %d\n",cluster_indices.size());
	distances.growTo(original_weights.size()-1);
	for(uint64_t i = 0; i < original_weights.size()-1; i++) {
	  distances[i] = original_weights[i+1] - original_weights[i];
	}
}

void Cluster_DivisiveMaxSeparate::clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) {
	if(max_c >= c) {
	  replaceWeights(formula,cluster_indices[c-1]);
	}
	else {
	  if(c > original_weights.size()) {
	    c = original_weights.size();
	  }	
	  vector<uint64_t> temp;
	  uint64_t previous_index = max_c-1;
	  uint64_t low_index, high_index, max_distance, max_index;
	  for(uint64_t i = max_c; i < c; i++) {
	    temp.clear();
	    max_distance = 0;
	    max_index = 0;
	    for(uint64_t j = 0; j < cluster_indices[previous_index].size(); j++) {
	      low_index = cluster_indices[previous_index][j];
	      high_index = (j == cluster_indices[previous_index].size()-1 ? original_weights.size()-1 : cluster_indices[previous_index][j+1]-1);
	      printf("LI : %d HI : %d\n",low_index,high_index);
	      for(uint64_t k = low_index; k < high_index; k++) {
	      	if(distances[k] >= max_distance) {
	      		printf("FOUND K : %d\n",k);
	      		max_distance = distances[k];
	      		max_index = k+1; // TODO BE VERY CAREFUL FOR CHECKING LAST ELEMENT! TODO CHECK!
	      	}
	      }
	    }
	    for(uint64_t j = 0; j < cluster_indices[previous_index].size(); j++) {
	      if(cluster_indices[previous_index][j] > max_index) {
	        temp.push_back(max_index);
	      }
        temp.push_back(cluster_indices[previous_index][j]);
	    }
	    if(max_index >= cluster_indices[previous_index][cluster_indices[previous_index].size()-1]) {
	      temp.push_back(max_index);
	    }
	    cluster_indices.push_back(temp);
	    previous_index = i;
	  }
	  for(int i1 = 0; i1 < cluster_indices.size(); i1++) {
	  	for(int i2 = 0; i2 < cluster_indices[i1].size(); i2++) {
	  		printf("%d ",cluster_indices[i1][i2]);
	  	}
	  	printf("\n");
	  }
	  replaceWeights(formula,cluster_indices[c-1]);
	}
}
