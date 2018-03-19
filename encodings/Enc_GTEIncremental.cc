#include "Enc_GTEIncremental.h"
#include <algorithm>
#include <numeric>
using namespace openwbo;

struct less_than_wlitt {
  inline bool operator()(const wlitt &wl1, const wlitt &wl2) {
    return (wl1.weight < wl2.weight);
  }
};

Lit GTEIncremental::getNewLit(Solver *S) {
  Lit p = mkLit(S->nVars(), false);
  newSATVariable(S);
  nb_variables++;
  return p;
}

Lit GTEIncremental::get_var(Solver *S, wlit_mapt &oliterals, uint64_t weight) {
  wlit_mapt::iterator it = oliterals.find(weight);
  if (it == oliterals.end()) {
    Lit v = getNewLit(S);
    oliterals[weight] = v;
  }
  return oliterals[weight];
}

bool GTEIncremental::encodeLeq(uint64_t k, Solver *S, const weightedlitst &iliterals,
                    wlit_mapt &oliterals) {

  if (iliterals.size() == 0 || k == 0)
    return false;

  if (iliterals.size() == 1) {

    oliterals.insert(
        wlit_pairt(iliterals.front().weight, iliterals.front().lit));
    return true;
  }

  unsigned int size = iliterals.size();

  // formulat lformula,rformula;
  weightedlitst linputs, rinputs;
  wlit_mapt loutputs, routputs;

  unsigned int lsize = size >> 1;
  // unsigned int rsize=size-lsize;
  weightedlitst::const_iterator myit = iliterals.begin();
  weightedlitst::const_iterator myit1 = myit + lsize;
  weightedlitst::const_iterator myit2 = iliterals.end();

  linputs.insert(linputs.begin(), myit, myit1);
  rinputs.insert(rinputs.begin(), myit1, myit2);

  /*wlitt init_wlit;
  init_wlit.lit = lit_Undef;
  init_wlit.weight=0;*/
  wlit_sumt wlit_sum;
  uint64_t lk = std::accumulate(linputs.begin(), linputs.end(), 0, wlit_sum);
  uint64_t rk = std::accumulate(rinputs.begin(), rinputs.end(), 0, wlit_sum);

  lk = k >= lk ? lk : k;
  rk = k >= rk ? rk : k;

  bool result = encodeLeq(lk, S, linputs, loutputs);
  if (!result)
    return result;
  result = result && encodeLeq(rk, S, rinputs, routputs);
  if (!result)
    return result;

  {
    assert(!loutputs.empty());

    for (wlit_mapt::iterator mit = loutputs.begin(); mit != loutputs.end();
         mit++) {

      if (mit->first > k) {
        addBinaryClause(S, ~mit->second, get_var(S, oliterals, k));
        nb_clauses++;
      } else {
        addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
        nb_clauses++;
        // clause.push_back(get_var(auxvars,oliterals,l.first));
      }

      // formula.push_back(std::move(clause));
    }
  }

  {
    assert(!routputs.empty());
    for (wlit_mapt::iterator mit = routputs.begin(); mit != routputs.end();
         mit++) {

      if (mit->first > k) {
        addBinaryClause(S, ~mit->second, get_var(S, oliterals, k));
        nb_clauses++;
        // clause.push_back(get_var(auxvars,oliterals,k));
      } else {
        addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
        nb_clauses++;
        // clause.push_back(get_var(auxvars,oliterals,r.first));
      }

      // formula.push_back(std::move(clause));
    }
  }

  // if(!lformula.empty() && !rformula.empty())
  {
    for (wlit_mapt::iterator lit = loutputs.begin(); lit != loutputs.end();
         lit++) {
      for (wlit_mapt::iterator rit = routputs.begin(); rit != routputs.end();
           rit++) {
        /*clauset clause;
        clause.push_back(-l.second);
        clause.push_back(-r.second);*/
        uint64_t tw = lit->first + rit->first;
        if (tw > k) {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, k));
          nb_clauses++;
          // clause.push_back(get_var(auxvars,oliterals,k));
        } else {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, tw));
          nb_clauses++;
          // clause.push_back(get_var(auxvars,oliterals,tw));
        }

        // formula.push_back(std::move(clause));
      }
    }
  }
  
  return true;
}

bool GTEIncremental::encodeLeqIncremental(uint64_t k, Solver *S, const weightedlitst &iliterals,
                    wlit_mapt &oliterals) {

  if (iliterals.size() == 0 || k == 0)
    return false;

  if (iliterals.size() == 1) {

    oliterals.insert(
        wlit_pairt(iliterals.front().weight, iliterals.front().lit));
    return true;
  }

  unsigned int size = iliterals.size();

  // formulat lformula,rformula;
  weightedlitst linputs, rinputs;
  wlit_mapt loutputs, routputs;

  unsigned int lsize = size >> 1;
  // unsigned int rsize=size-lsize;
  weightedlitst::const_iterator myit = iliterals.begin();
  weightedlitst::const_iterator myit1 = myit + lsize;
  weightedlitst::const_iterator myit2 = iliterals.end();

  linputs.insert(linputs.begin(), myit, myit1);
  rinputs.insert(rinputs.begin(), myit1, myit2);

  /*wlitt init_wlit;
  init_wlit.lit = lit_Undef;
  init_wlit.weight=0;*/
  wlit_sumt wlit_sum;
  uint64_t lk = std::accumulate(linputs.begin(), linputs.end(), 0, wlit_sum);
  uint64_t rk = std::accumulate(rinputs.begin(), rinputs.end(), 0, wlit_sum);

  lk = k >= lk ? lk : k;
  rk = k >= rk ? rk : k;

  bool result = encodeLeqIncremental(lk, S, linputs, loutputs);
  if (!result)
    return result;
  result = result && encodeLeqIncremental(rk, S, rinputs, routputs);
  if (!result)
    return result;
    
  bool added_first_above_k = false;

  {
    assert(!loutputs.empty());

    for (wlit_mapt::iterator mit = loutputs.begin(); mit != loutputs.end();
         mit++) {
      // sending copy of weight to parent in tree - left side
      addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
      nb_clauses++;
      // clause.push_back(get_var(auxvars,oliterals,l.first));

      // formula.push_back(std::move(clause));
    }
  }

  {
    assert(!routputs.empty());
    for (wlit_mapt::iterator mit = routputs.begin(); mit != routputs.end();
         mit++) {
      // sending copy of weight to parent in tree - right side
      addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
      nb_clauses++;
      // clause.push_back(get_var(auxvars,oliterals,r.first));

      // formula.push_back(std::move(clause));
    }
  }

  // if(!lformula.empty() && !rformula.empty())
  {
    // sending pairwise sums to parent
    for (wlit_mapt::iterator lit = loutputs.begin(); lit != loutputs.end();
         lit++) {
      for (wlit_mapt::iterator rit = routputs.begin(); rit != routputs.end();
           rit++) {
        /*clauset clause;
        clause.push_back(-l.second);
        clause.push_back(-r.second);*/
        uint64_t tw = lit->first + rit->first;
        if (tw > k && !added_first_above_k) {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, tw)); // TODO - check
          nb_clauses++;
          added_first_above_k = true;
          // clause.push_back(get_var(auxvars,oliterals,k));
        } else if(tw <= k) {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, tw));
          nb_clauses++;
          // clause.push_back(get_var(auxvars,oliterals,tw));
        }

        // formula.push_back(std::move(clause));
      }
    }
  }
  
  for(wlit_mapt::reverse_iterator oit = ++(oliterals.rbegin());
      oit != oliterals.rend(); oit++) {
    wlit_mapt::reverse_iterator implied_lit = oit;
    --implied_lit;
    addBinaryClause(S, ~oit->second, implied_lit->second);
    nb_clauses++;
  }

  return true;
}

bool GTEIncremental::extendTree(uint64_t k, Solver *S, const weightedlitst &iliterals,
                    wlit_mapt &oliterals, uint64_t old_rhs) {
                      
  assert(k > old_rhs);
  
  if (iliterals.size() == 0 || k == 0)
    return false;

  if (iliterals.size() == 1) {

    oliterals.insert(
        wlit_pairt(iliterals.front().weight, iliterals.front().lit));
    return true;
  }

  unsigned int size = iliterals.size();

  // formulat lformula,rformula;
  weightedlitst linputs, rinputs;
  wlit_mapt loutputs, routputs;

  unsigned int lsize = size >> 1;
  // unsigned int rsize=size-lsize;
  weightedlitst::const_iterator myit = iliterals.begin();
  weightedlitst::const_iterator myit1 = myit + lsize;
  weightedlitst::const_iterator myit2 = iliterals.end();

  linputs.insert(linputs.begin(), myit, myit1);
  rinputs.insert(rinputs.begin(), myit1, myit2);

  /*wlitt init_wlit;
  init_wlit.lit = lit_Undef;
  init_wlit.weight=0;*/
  wlit_sumt wlit_sum;
  uint64_t lk = std::accumulate(linputs.begin(), linputs.end(), 0, wlit_sum);
  uint64_t rk = std::accumulate(rinputs.begin(), rinputs.end(), 0, wlit_sum);

  lk = k >= lk ? lk : k;
  rk = k >= rk ? rk : k;

  bool result = encodeLeqIncremental(lk, S, linputs, loutputs);
  if (!result)
    return result;
  result = result && encodeLeqIncremental(rk, S, rinputs, routputs);
  if (!result)
    return result;
    
  bool added_first_above_k = false;

  {
    assert(!loutputs.empty());

    for (wlit_mapt::iterator mit = loutputs.begin(); mit != loutputs.end();
         mit++) {
      // sending copy of weight to parent in tree - left side
    
      addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
      nb_clauses++;

    }
  }

  {
    assert(!routputs.empty());
    for (wlit_mapt::iterator mit = routputs.begin(); mit != routputs.end();
         mit++) {
      // sending copy of weight to parent in tree - right side
    
      addBinaryClause(S, ~mit->second, get_var(S, oliterals, mit->first));
      nb_clauses++;
    
      // formula.push_back(std::move(clause));
    }
  }

  // if(!lformula.empty() && !rformula.empty())
  {
    // sending pairwise sums to parent
    for (wlit_mapt::iterator lit = loutputs.begin(); lit != loutputs.end();
         lit++) {
      for (wlit_mapt::iterator rit = routputs.begin(); rit != routputs.end();
           rit++) {
        /*clauset clause;
        clause.push_back(-l.second);
        clause.push_back(-r.second);*/
        uint64_t tw = lit->first + rit->first;
        if (tw > k && !added_first_above_k) {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, tw)); // TODO - check
          nb_clauses++;
          added_first_above_k = true;
          // clause.push_back(get_var(auxvars,oliterals,k));
        } else if(tw <= k && tw > old_rhs) {
          addTernaryClause(S, ~lit->second, ~rit->second,
                           get_var(S, oliterals, tw));
          nb_clauses++;
          // clause.push_back(get_var(auxvars,oliterals,tw));
        }

        // formula.push_back(std::move(clause));
      }
    }
  }
  
  for(wlit_mapt::reverse_iterator oit = ++(oliterals.rbegin());
      oit != oliterals.rend(); oit++) {
    wlit_mapt::reverse_iterator implied_lit = oit;
    --implied_lit;
    if(oit->first > old_rhs) {
      addBinaryClause(S, ~oit->second, implied_lit->second);
      nb_clauses++;
    }
   
  }

  return true;
}


void GTEIncremental::encode(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs,
                 uint64_t rhs) {
  // FIXME: do not change coeffs in this method. Make coeffs const.

  // If the rhs is larger than INT32_MAX is not feasible to encode this
  // pseudo-Boolean constraint to CNF.
  // CHANGED TO INT64_MAX for now - Sukrut
  if (rhs >= INT64_MAX) {
    printf("c Overflow in the Encoding\n");
    printf("s UNKNOWN\n");
    exit(_ERROR_);
  }

  hasEncoding = false;
  nb_variables = 0;
  nb_clauses = 0;

  vec<Lit> simp_lits;
  vec<uint64_t> simp_coeffs;
  lits.copyTo(simp_lits);
  coeffs.copyTo(simp_coeffs);

  lits.clear();
  coeffs.clear();

  // Fix literals that have a coeff larger than rhs.
  for (int i = 0; i < simp_lits.size(); i++) {
    if (simp_coeffs[i] == 0)
      continue;
	
	// CHANGED TO INT64_MAX for now - Sukrut
    if (simp_coeffs[i] >= INT64_MAX) {
      printf("c Overflow in the Encoding\n");
      printf("s UNKNOWN\n");
      exit(_ERROR_);
    }

    if (simp_coeffs[i] <= (unsigned)rhs) {
      lits.push(simp_lits[i]);
      coeffs.push(simp_coeffs[i]);
    } else
      addUnitClause(S, ~simp_lits[i]);
  }

  if (lits.size() == 1) { // TODO - how is this sound? - Sukrut
    // addUnitClause(S, ~lits[0]);
    return;
  }

  if (lits.size() == 0)
    return;

  weightedlitst iliterals;
  for (int i = 0; i < lits.size(); i++) {
    wlitt wl;
    wl.lit = lits[i];
    wl.weight = coeffs[i];
    iliterals.push_back(wl);
  }
  less_than_wlitt lt_wlit;
  std::sort(iliterals.begin(), iliterals.end(), lt_wlit);
  if(incremental_strategy == _INCREMENTAL_ITERATIVE_) {
    encodeLeqIncremental(rhs + 1, S, iliterals, pb_oliterals);
  } else {
    encodeLeq(rhs + 1, S, iliterals, pb_oliterals);
  }
  

  for (wlit_mapt::reverse_iterator rit = pb_oliterals.rbegin();
       rit != pb_oliterals.rend(); rit++) {
    if (rit->first > rhs) {
      addUnitClause(S, ~rit->second);
    } else {
      break;
    }
  }
  // addUnitClause(S,~pb_oliterals.rbegin()->second);
  /*
  if (pb_oliterals.rbegin()->first != rhs+1){
        printf("%d - %d\n",pb_oliterals.rbegin()->first,rhs);
        for(wlit_mapt::reverse_iterator
  rit=pb_oliterals.rbegin();rit!=pb_oliterals.rend();rit++)
  {
        printf("rit->first %d\n",rit->first);
  }
  }
  */
  // assert (pb_oliterals.rbegin()->first == rhs+1);
  // printLit(~pb_oliterals.rbegin()->second);
  /* ... PUT CODE HERE FOR CREATING THE ENCODING ... */
  /* ... do not forget to sort the coefficients so that GTE is more efficient
   * ... */

  current_pb_rhs = rhs;
  hasEncoding = true;
}

void GTEIncremental::update(Solver *S, uint64_t rhs) {

  assert(hasEncoding);
  for (wlit_mapt::reverse_iterator rit = pb_oliterals.rbegin();
       rit != pb_oliterals.rend(); rit++) {
    if (rit->first > current_pb_rhs)
      continue;
    if (rit->first > rhs) {
      addUnitClause(S, ~rit->second);
    } else {
      break;
    }
  }
  /* ... PUT CODE HERE TO UPDATE THE RHS OF AN ALREADY EXISTING ENCODING ... */
  
  // add missing literals and clauses
  if(incremental_strategy == _INCREMENTAL_ITERATIVE) {
    
  }

  current_pb_rhs = rhs;
}

void GTEIncremental::join(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs,
                 uint64_t rhs) {
	
  assert(incremental_strategy == _INCREMENTAL_ITERATIVE_);
	wlit_mapt left_pb_oliterals;
	left_pb_oliterals.insert(pb_oliterals.begin(), pb_oliterals.end());
	pb_oliterals.clear();
  uint64_t old_pb = current_pb_rhs;

  encode(S, lits, coeffs, rhs);
 
  wlit_mapt right_pb_oliterals;
  right_pb_oliterals.insert(pb_oliterals.begin(), pb_oliterals.end());
  pb_oliterals.clear();
  
  
  
  
}

void GTEIncremental::
