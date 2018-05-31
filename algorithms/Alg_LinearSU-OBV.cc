/*!
 * \author Ruben Martins - ruben@sat.inesc-id.pt
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

#include "Alg_LinearSU-OBV.h"
#include <cstdlib>
#include <ctime>     
#include <vector>
#include <algorithm> 

using namespace openwbo;

uint64_t LinearSU_OBV::MrsBeaver(Solver * solver, int iterations, int conflicts, uint64_t ub){

  std::srand ( 1971603567 );
  std::vector<Lit> outputs;
  
  for (int i = 0; i < objFunction.size(); i++)
    outputs.push_back(objFunction[i]);

  vec<Lit> dummy;
  lbool res = searchSATSolver(solver, dummy);
  if (res != l_True)
    return 0;

  uint64_t current_ub = computeCostModel(solver->model);
  saveModel(solver->model);
  printf("o %" PRId64 "\n", current_ub);
  
  vec<lbool> original_model;
  solver->model.copyTo(original_model);

  // encoder.setCardEncoding(_CARD_TOTALIZER_);
  // if (!encoder.hasCardEncoding())
  //     encoder.encodeCardinality(solver, objFunction, current_ub - 1);

  //   for (int i = 0; i < encoder.getOutputs().size(); i++)
  //     outputs.push_back(encoder.getOutputs()[i]);
  // while (outputs.size() > ub){
  //   outputs.pop_back();
  // }
  // std::reverse(outputs.begin(),outputs.end());

  
  for (int t = 0; t < iterations; t++){
    printf("iteration = %d\n",t);
    model_all.clear();
    original_model.copyTo(model_all);
    //std::reverse(outputs.begin(),outputs.end());
    //std::random_shuffle ( outputs.begin(), outputs.end() );

    //current_ub = obv_bs(solver, outputs, current_ub, conflicts);
    current_ub = ums_obv_bs(solver, outputs, current_ub, conflicts);

    // if (t % 2 == 0){
    //   current_ub = ums_obv_bs(solver, outputs, current_ub, conflicts);
    // } else {
    //   current_ub = obv_bs(solver, outputs, current_ub, conflicts);
    // }

    // if (t % 4 == 0){
    //   // suffle
    //   std::random_shuffle ( outputs.begin(), outputs.end() );
    // } else {
    //   // reverse
    //     std::reverse(outputs.begin(),outputs.end());
    // }
    

  }

  return current_ub;

}

uint64_t LinearSU_OBV::obv_bs(Solver * solver, std::vector<Lit>& outputs, uint64_t ub, int conflicts){

  vec<Lit> assumptions;
  vec<lbool> current_model;
  assert (model_all.size() != 0);
  model_all.copyTo(current_model);
  uint64_t last_ub = ub;
  int limit = 100;
  int current_limit = 0;

  for (int i =0; i < outputs.size(); i++){
    // if (current_limit > limit)
    //   break;
    //printf("bit %d\n",i);
    // printf("model size = %d\n",model_all.size());
    // printf("v = %d\n",var(outputs[i]));
    // printf("vars = %d\n",solver->nVars());
    if (current_model[var(outputs[i])] == l_False){
      //printf("bit %d with value true! rhs = %d\n",i,outputs.size()-i);
      assumptions.push(~outputs[i]);
    } else {
      vec<Lit> current_assumptions;
      assumptions.copyTo(current_assumptions);
      current_assumptions.push(~outputs[i]);

      for (int i = 0; i < outputs.size(); i++){
        solver->setPolarity(var(outputs[i]),true);
      }
      //printf("calling a SAT solver!\n");
      solver->setConfBudget(conflicts);
      lbool res = searchSATSolver(solver, current_assumptions);

      if (res == l_True){
        current_limit = 0;
        //printf("SAT!\n");
        assumptions.push(~outputs[i]);
        // check if new model is better
        uint64_t newCost = computeCostModel(solver->model);
        //printf("c o %" PRId64 "\n", newCost);
        if (newCost < last_ub){
          saveModel(solver->model);
          printf("o %" PRId64 "\n", newCost);
          last_ub = newCost;
        }
        current_model.clear();
        solver->model.copyTo(current_model);
      } else {
        // if (res == l_False){
        //   //printf("c unsat!\n");
        // } else {
        //   //printf("c unknown!\n");
        // }
        current_limit++;
        assumptions.push(outputs[i]);
      }
    }
  }
  solver->budgetOff();
  return last_ub;
}

uint64_t LinearSU_OBV::ums_obv_bs(Solver * solver, std::vector<Lit>& outputs, uint64_t ub, int conflicts){
  vec<Lit> assumptions;
  vec<lbool> current_model;
  assert (model.size() != 0);
  model.copyTo(current_model);
  uint64_t last_ub = ub;
  int limit = 100;
  int current_limit = 0;
  
  std::vector<Lit> outputs_mod;
  for (int i = 0; i < outputs.size(); i++){
    outputs_mod.push_back(outputs[i]);
  }

  for (int i =0; i < outputs_mod.size(); i++){
    // if (current_limit > limit)
    //   break;
    //printf("bit %d\n",i);
    if (current_model[var(outputs_mod[i])] == l_False){
      assumptions.push(~outputs_mod[i]);
    } else {
      vec<Lit> current_assumptions;
      assumptions.copyTo(current_assumptions);
      current_assumptions.push(~outputs_mod[i]);

      for (int i = 0; i < outputs.size(); i++){
        solver->setPolarity(var(outputs[i]),true);
      }
      solver->setConfBudget(conflicts);
      lbool res = searchSATSolver(solver, current_assumptions);

      // move bits
      if (res == l_True){
        current_limit = 0;
        //printf("model!\n");
        int k = i+1;
        for (int j = i+1; j < outputs_mod.size(); j++){
          if (solver->model[var(outputs_mod[j])] == l_False){
            //printf("var is false!\n");
            if (k!=j){
              //printf("swapping bit %d with bit%d\n",j,k);
              Lit a = outputs_mod[k];
              outputs_mod[k] = outputs_mod[j];
              outputs_mod[j] = a;
            }
            k++;
          }
        }
      }
      if (res == l_True){
        assumptions.push(~outputs_mod[i]);
        // check if new model is better
        uint64_t newCost = computeCostModel(solver->model);
        if (newCost < last_ub){
          saveModel(solver->model);
          printf("o %" PRId64 "\n", newCost);
          last_ub = newCost;
        }
        current_model.clear();
        solver->model.copyTo(current_model);
      } else {
        current_limit++;
        assumptions.push(outputs_mod[i]);
      }
    }
  }
  solver->budgetOff();
  return last_ub;
}

/*_________________________________________________________________________________________________
  |
  |  normalSearch : [void] ->  [void]
  |
  |  Description:
  |
  |    Linear search algorithm.
  |
  |  For further details see:
  |    *  Daniel Le Berre, Anne Parrain: The Sat4j library, release 2.2. JSAT
  |       7(2-3): 59-6 (2010)
  |    *  Miyuki Koshimura, Tong Zhang, Hiroshi Fujita, Ryuzo Hasegawa: QMaxSAT:
  |       A Partial Max-SAT Solver. JSAT 8(1/2): 95-100 (2012)
  |
  |  Post-conditions:
  |    * 'ubCost' is updated.
  |    * 'nbSatisfiable' is updated.
  |    * 'nbCores' is updated.
  |
  |________________________________________________________________________________________________@*/
void LinearSU_OBV::normalSearch() {

  lbool res = l_True;

  initRelaxation();
  solver = rebuildSolver();
  bool mrsb = true;
  uint64_t newCost = UINT64_MAX;

  while (res == l_True) {

    vec<Lit> dummy;
    // Do not use preprocessing for linear search algorithm.
    // NOTE: When preprocessing is enabled the SAT solver simplifies the
    // relaxation variables which leads to incorrect results.
    //res = searchSATSolver(solver, dummy);

    // invoke Mrs. Beaver
    if (mrsb) {
          printf("c using Mrs. Beaver preprocessor\n");
          newCost = MrsBeaver(solver, 1, 1000, 0);
          printf("c after Mrs. Beaver ub %" PRId64 "\n", newCost); 
          //ubCost = newCost;
    } else {
      res = searchSATSolver(solver, dummy);
    }

    if (model.size() == 0){
      printAnswer(_UNSATISFIABLE_);
      exit(_UNSATISFIABLE_);
    }

    if (res == l_True) {
      nbSatisfiable++;
      if  (!mrsb){
        newCost = computeCostModel(solver->model);
        saveModel(solver->model);
      } else {
        mrsb = false;
      }
      if (maxsat_formula->getFormat() == _FORMAT_PB_) {
        // optimization problem
        if (maxsat_formula->getObjFunction() != NULL) {
          printf("o %" PRId64 "\n", newCost + off_set);
        }
      } else
        printf("o %" PRId64 "\n", newCost + off_set); 

      if (newCost == 0) {
        // If there is a model with value 0 then it is an optimal model
        ubCost = newCost;

        if (maxsat_formula->getFormat() == _FORMAT_PB_ &&
            maxsat_formula->getObjFunction() == NULL) {
          printAnswer(_SATISFIABLE_);
          exit(_SATISFIABLE_);
        } else {
          printAnswer(_OPTIMUM_);
          exit(_OPTIMUM_);
        }

      } else {
        if (maxsat_formula->getProblemType() == _WEIGHTED_) {
          if (!encoder.hasPBEncoding())
            encoder.encodePB(solver, objFunction, coeffs, newCost - 1);
          else
            encoder.updatePB(solver, newCost - 1);
        } else {
          // Unweighted.
          if (!encoder.hasCardEncoding())
            encoder.encodeCardinality(solver, objFunction, newCost - 1);
          else
            encoder.updateCardinality(solver, newCost - 1);
        }

        ubCost = newCost;
        }
    } else {
      nbCores++;
      if (model.size() == 0) {
        assert(nbSatisfiable == 0);
        // If no model was found then the MaxSAT formula is unsatisfiable
        printAnswer(_UNSATISFIABLE_);
        exit(_UNSATISFIABLE_);
      } else {
        printAnswer(_OPTIMUM_);
        exit(_OPTIMUM_);
      }
    }
  }
}

// Public search method
void LinearSU_OBV::search() {

  assert (maxsat_formula->getProblemType() == _UNWEIGHTED_);

  printConfiguration(is_bmo, maxsat_formula->getProblemType());
  normalSearch();
}

/************************************************************************************************
 //
 // Rebuild MaxSAT solver
 //
 ************************************************************************************************/

/*_________________________________________________________________________________________________
  |
  |  rebuildSolver : (minWeight : int)  ->  [Solver *]
  |
  |  Description:
  |
  |    Rebuilds a SAT solver with the current MaxSAT formula.
  |    If a weight is specified, then it only considers soft clauses with weight
  |    smaller than the specified weight.
  |    NOTE: a weight is specified in the 'bmo' approach.
  |
  |________________________________________________________________________________________________@*/
Solver *LinearSU_OBV::rebuildSolver(uint64_t min_weight) {

  vec<bool> seen;
  seen.growTo(maxsat_formula->nVars(), false);

  Solver *S = newSATSolver();

  for (int i = 0; i < maxsat_formula->nVars(); i++)
    newSATVariable(S);

  for (int i = 0; i < maxsat_formula->nHard(); i++)
    S->addClause(maxsat_formula->getHardClause(i).clause);

  for (int i = 0; i < maxsat_formula->nPB(); i++) {
    Encoder *enc = new Encoder(_INCREMENTAL_NONE_, _CARD_MTOTALIZER_,
                               _AMO_LADDER_, _PB_GTE_);

    // Make sure the PB is on the form <=
    // if (maxsat_formula->getPBConstraint(i)->_sign)
    //  maxsat_formula->getPBConstraint(i)->changeSign();
    assert(maxsat_formula->getPBConstraint(i)->_sign);

    enc->encodePB(S, maxsat_formula->getPBConstraint(i)->_lits,
                  maxsat_formula->getPBConstraint(i)->_coeffs,
                  maxsat_formula->getPBConstraint(i)->_rhs);

    delete enc;
  }

  for (int i = 0; i < maxsat_formula->nCard(); i++) {
    Encoder *enc = new Encoder(_INCREMENTAL_NONE_, _CARD_MTOTALIZER_,
                               _AMO_LADDER_, _PB_GTE_);

    if (maxsat_formula->getCardinalityConstraint(i)->_rhs == 1) {
      enc->encodeAMO(S, maxsat_formula->getCardinalityConstraint(i)->_lits);
    } else {

      enc->encodeCardinality(S,
                             maxsat_formula->getCardinalityConstraint(i)->_lits,
                             maxsat_formula->getCardinalityConstraint(i)->_rhs);
    }

    delete enc;
  }

  vec<Lit> clause;
  for (int i = 0; i < maxsat_formula->nSoft(); i++) {
    if (maxsat_formula->getSoftClause(i).weight < min_weight)
      continue;

    clause.clear();
    maxsat_formula->getSoftClause(i).clause.copyTo(clause);

    for (int j = 0; j < maxsat_formula->getSoftClause(i).relaxation_vars.size();
         j++) {
      clause.push(maxsat_formula->getSoftClause(i).relaxation_vars[j]);
    }

    S->addClause(clause);
  }

  return S;
}

/*_________________________________________________________________________________________________
  |
  |  rebuildBMO : (functions : int)  ->  [Solver *]
  |
  |  Description:
  |
  |    Rebuilds a SAT solver with the current MaxSAT formula.
  |    Only considers soft clauses with the weight of the current
  |    lexicographical optimization weight ('currentWeight')
  |    For each function already computed in the BMO algorithm it encodes the
  |    respective cardinality constraint.
  |
  |________________________________________________________________________________________________@*/
Solver *LinearSU_OBV::rebuildBMO(vec<vec<Lit>> &functions, vec<int> &rhs,
                             uint64_t currentWeight) {

  assert(functions.size() == rhs.size());

  Solver *S = rebuildSolver(currentWeight);

  objFunction.clear();
  coeffs.clear();
  for (int i = 0; i < maxsat_formula->nSoft(); i++) {
    if (maxsat_formula->getSoftClause(i).weight == currentWeight) {
      objFunction.push(maxsat_formula->getSoftClause(i).relaxation_vars[0]);
      coeffs.push(maxsat_formula->getSoftClause(i).weight);
    }
  }

  for (int i = 0; i < functions.size(); i++)
    encoder.encodeCardinality(S, functions[i], rhs[i]);

  return S;
}

/************************************************************************************************
 //
 // Other protected methods
 //
 ************************************************************************************************/

/*_________________________________________________________________________________________________
  |
  |  initRelaxation : (objective : vec<Lit>&) (weights : vec<int>&)  ->  [void]
  |
  |  Description:
  |
  |    Initializes the relaxation variables by adding a fresh variable to the
  |    'relaxationVars' of each soft clause.
  |
  |  Post-conditions:
  |    * 'objFunction' contains all relaxation variables that were added to soft
  |       clauses.
  |    * 'coeffs' contains the weights of all soft clauses.
  |
  |________________________________________________________________________________________________@*/
void LinearSU_OBV::initRelaxation() {
  for (int i = 0; i < maxsat_formula->nSoft(); i++) {
    Lit l = maxsat_formula->newLiteral();
    maxsat_formula->getSoftClause(i).relaxation_vars.push(l);
    objFunction.push(l);
    coeffs.push(maxsat_formula->getSoftClause(i).weight);
  }
}

// Print LinearSU configuration.
void LinearSU_OBV::print_LinearSU_configuration() {
  printf("c |  Algorithm: %23s                                             "
         "                      |\n",
         "LinearSU");

  if (maxsat_formula->getProblemType() == _WEIGHTED_) {
    if (bmoMode)
      printf("c |  BMO strategy: %20s                      "
             "                                             |\n",
             "On");
    else
      printf("c |  BMO strategy: %20s                      "
             "                                             |\n",
             "Off");

    if (bmoMode) {
      if (is_bmo)
        printf("c |  BMO search: %22s                      "
               "                                             |\n",
               "Yes");
      else
        printf("c |  BMO search: %22s                      "
               "                                             |\n",
               "No");
    }
  }
}
