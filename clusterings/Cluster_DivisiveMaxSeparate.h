#ifndef Cluster_DivisiveMaxSeparate_h
#define Cluster_DivisiveMaxSeparate_h

#ifdef SIMP
#include "simp/SimpSolver.h"
#else
#include "core/Solver.h"
#endif

#include "Cluster.h"
#include <vector>

using NSPACE::vec;
using std::vector;

namespace openwbo {

class Cluster_DivisiveMaxSeparate : public Cluster {
  
public:
  vec<cluster_index> cluster_indices;
  uint64_t max_c;
  vec<uint64_t> distances;
  
  Cluster_DivisiveMaxSeparate(MaxSATFormulaExtended *formula, Statistics cluster_stat);
  void clusterWeights(MaxSATFormulaExtended *formula, uint64_t c);
  
};

}

#endif
