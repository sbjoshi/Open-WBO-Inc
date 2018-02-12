#ifndef Cluster_DivisiveMaxSeparate_h
#define Cluster_DivisiveMaxSeparate_h

using NSPACE::vec;

namespace openwbo {

class Cluster_DivisiveMaxSeparate : public Cluster {
  
public:
  vec<vec<uint64_t> > cluster_indices;
  uint64_t max_c;
  vec<uint64_t> distances;
  
  Cluster_DivisiveMaxSeparate(MaxSATFormulaCluster *formula, Statistics cluster_stat);
  void clusterWeights(MaxSATFormulaCluster *formula, uint64_t c);
  
};

}

#endif
