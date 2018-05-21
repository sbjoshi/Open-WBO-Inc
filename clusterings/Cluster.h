/*!
 * \author Sukrut Rao - cs15btech11036@iith.ac.in
 *
 * @section LICENSE
 *
 * Open-WBO, Copyright (c) 2013-2017, Ruben Martins, Vasco Manquinho, Ines Lynce
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef Cluster_h
#define Cluster_h

#ifdef SIMP
#include "simp/SimpSolver.h"
#else
#include "core/Solver.h"
#endif

#include "../MaxSATFormulaExtended.h"
#include "SequenceStatistic.h"
#include <vector>

using NSPACE::vec;
using std::vector;

namespace openwbo {

struct cluster_index {
  uint64_t index;
  bool all_equal_in_cluster;
};

class Cluster {

public:
  vec<uint64_t> original_weights; // store the original weights
  Statistics cluster_statistic;
  SequenceStatistic statistic_finder;
  uint64_t num_clauses;

  Cluster(MaxSATFormulaExtended *formula, Statistics cluster_statistic);
  virtual void clusterWeights(MaxSATFormulaExtended *formula, uint64_t c) = 0;
  void saveWeights(MaxSATFormulaExtended *formula);
  void restoreWeights(MaxSATFormulaExtended *formula);
  void replaceWeights(MaxSATFormulaExtended *formula, vec<uint64_t> &clusters);
  void replaceWeights(MaxSATFormulaExtended *formula,
                      vec<cluster_index> &clusters);
  uint64_t getOriginalWeight(int index);

}; // class Cluster

} // namespace openwbo

#endif
