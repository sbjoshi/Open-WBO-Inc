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

#include "Cluster.h"

#include <cstdio>

using namespace openwbo;

Cluster::Cluster(MaxSATFormulaExtended *formula, Statistics cluster_stat) {
  assert(formula != NULL);
  formula->sortSoftClauses();
  saveWeights(formula);
  cluster_statistic = cluster_stat;
  statistic_finder.setStatistic(cluster_statistic);
  num_clauses = formula->getSoftClauses().size();
}

/*_________________________________________________________________________________________________
  |
  |  saveWeights : (formula : MaxSATFormulaExtended *) ->  [void]
  |
  |  Description:
  |
  |    Saves the weights of 'formula' in the object. If formula is NULL, the
  |    function has no effect.
  |
  |  Pre-conditions:
  |    * None.
  |
  |  Post-conditions:
  |    * 'formula' is not modified.
  |
  |________________________________________________________________________________________________@*/
void Cluster::saveWeights(MaxSATFormulaExtended *formula) {
  if (formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  original_weights.growTo(soft_clauses.size());
  for (int i = 0; i < soft_clauses.size(); i++) {
    original_weights[i] = soft_clauses[i].weight;
  }
}

/*_________________________________________________________________________________________________
  |
  |  restoreWeights : (formula : MaxSATFormulaExtended *) ->  [void]
  |
  |  Description:
  |
  |    Restores previously saved weights of 'formula' in the object, back to
  |    formula. If the formula is NULL, this function has no effect.
  |
  |  Pre-conditions:
  |    * 'saveWeights()' must have been previously called to store the weights
  |      of the formula in the object.
  |    * The formula used with 'saveWeights()' in the last call must be the same
  |      as pointed to by 'formula'.
  |
  |  Post-conditions:
  |    * The previously saved weights of 'formula' are restored.
  |
  |________________________________________________________________________________________________@*/
void Cluster::restoreWeights(MaxSATFormulaExtended *formula) {
  if (formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  assert(original_weights.size() != soft_clauses.size());
  for (int i = 0; i < soft_clauses.size(); i++) {
    soft_clauses[i].weight = original_weights[i];
  }
}

/*_________________________________________________________________________________________________
  |
  |  replaceWeights : (formula : MaxSATFormulaExtended *) (clusters :
  |                    vec<uint64_t> &) ->  [void]
  |
  |  Description:
  |
  |    Replaces the weights in 'formula' by clustered weights. Clusters are
  |    demarcated by indices in 'clusters', where each value in the vec
  |    represents the index of the start of a new cluster. The new weights are
  |    computed based on the statistic specified in the constructor. If
  |    'formula' is NULL, this function has no effect.
  |
  |  Pre-conditions:
  |    * 'clusters' be a sorted vec with all values unique and in the range of
  |      indices of the soft clauses in `formula`.
  |
  |  Post-conditions:
  |    * Weights in 'formula' are replaced with clustered weights. For every
  |      consecutive pair of value i and j in `clusters`, indices [i,j-1] in
  |      the soft clauses of the formula are replaced with a common weight based
  |      on the statistic. For the last value in `clusters`, all weights from
  |      that index till the last index are replaced with a common weight based
  |      on the statistic.
  |
  |________________________________________________________________________________________________@*/
void Cluster::replaceWeights(MaxSATFormulaExtended *formula,
                             vec<uint64_t> &clusters) {
  if (formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  uint64_t low_index, high_index, replacement_weight;
  for (uint64_t i = 0; i < clusters.size(); i++) {
    low_index = clusters[i];
    high_index = (i == clusters.size() - 1 ? original_weights.size() - 1
                                           : clusters[i + 1] - 1);
    replacement_weight = statistic_finder.getSequenceStatistic(
        original_weights, low_index, high_index);
    for (uint64_t j = low_index; j <= high_index; j++) {
      soft_clauses[j].weight = replacement_weight;
    }
  }
}

void Cluster::replaceWeights(MaxSATFormulaExtended *formula,
                             vec<cluster_index> &clusters) {
  if (formula == NULL) {
    return;
  }
  vec<Soft> &soft_clauses = formula->getSoftClauses();
  uint64_t low_index, high_index, replacement_weight;
  for (uint64_t i = 0; i < clusters.size(); i++) {
    if (clusters[i].all_equal_in_cluster) {
      continue;
    } // ASSUMPTION
    low_index = clusters[i].index;
    high_index = (i == clusters.size() - 1 ? original_weights.size() - 1
                                           : clusters[i + 1].index - 1);
    replacement_weight = statistic_finder.getSequenceStatistic(
        original_weights, low_index, high_index);
    for (uint64_t j = low_index; j <= high_index; j++) {
      soft_clauses[j].weight = replacement_weight;
    }
  }
}

uint64_t Cluster::getOriginalWeight(int index) {
  assert(index >= 0 && index < original_weights.size());
  return original_weights[index];
}
