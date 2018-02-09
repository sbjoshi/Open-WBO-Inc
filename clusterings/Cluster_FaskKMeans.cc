#include "Cluster_FastKMeans.h"

using namespace openwbo;

Cluster_FastKMeans::Cluster_FastKMeans(MaxSATFormula *formula,
    Statistics cluster_statistic)
      : Cluster(formula,cluster_statistic) {
  // growTo() vecs here?
}

uint64_t Cluster_FastKMeans::cost(uint64_t start_index, uint64_t end_index) {
  // Check if there is a better way to do this
  uint64_t centroid = statistic_finder(original_weights,start_index,end_index);
  
}
