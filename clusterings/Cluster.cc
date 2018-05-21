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
