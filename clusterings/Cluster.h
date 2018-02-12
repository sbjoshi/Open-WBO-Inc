#ifndef Cluster_h
#define Cluster_h

using NSPACE::vec;
using NSPACE::lit;

namespace openwbo {
  
class Cluster {
  
public:
  vec<uint64_t> original_weights; // store the original weights
  Statistics cluster_statistic;
  SequenceStatistic statistic_finder;
  uint64_t num_clauses;
  
  Cluster(MaxSATFormulaCluster *formula, Statistics cluster_statistic);
  virtual void clusterWeights(MaxSATFormulaCluster *formula, uint64_t c) = 0;
  void saveWeights(MaxSATFormulaCluster *formula);
  void restoreWeights(MaxSATFormulaCluster *formula);
  void replaceWeights(MaxSATFormulaCluster *formula, vec<uint64_t> clusters);
  
}; // class Cluster
  
} // namespace openwbo


#endif
