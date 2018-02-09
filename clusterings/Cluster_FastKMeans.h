#ifndef Cluster_FastKMeans_h
#define Cluster_FastKMeans_h

using NSPACE::vec;
using NSPACE::lit;

namespace openwbo {

class Cluster_FastKMeans : public Cluster {
  
public:
   vec<vec<uint64_t> > d_matrix; // n x c matrix to memoize
   vec<vec<uint64_t> > c_matrix;
   vec<vec<uint64_t> > t_matrix;
   
   Cluster_FastKMeans(MaxSATFormula *formula, Statistics cluster_statistic);
   uint64_t cost(uint64_t start_index, uint64_t end_index);
   void clusterWeights(MaxSATFormula *formula, uint64_t c);
   

};

}

#endif
