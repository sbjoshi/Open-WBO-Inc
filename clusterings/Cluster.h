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
  
  Cluster(MaxSATFormula *formula, Statistics cluster_statistic);
  virtual void clusterWeights(MaxSATFormula *formula, uint64_t c) = 0;
  void saveWeights(MaxSATFormula *formula);
  void restoreWeights(MaxSATFormula *formula);
  
}; // class Cluster
  
} // namespace openwbo


#endif
