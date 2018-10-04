/*!
 * \author Saurabh Joshi - sbjoshi@iith.ac.in
 *
 * @section LICENSE
 *
 * Open-WBO, Copyright (c) 2013-2017, Ruben Martins, Vasco Manquinho, Ines Lynce
 *           Copyright (c) 2015  Saurabh Joshi
 *           Copyright (c) 2018  Prateek Kumar, Sukrut Rao
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

#include "Enc_GTE.h"
#include <algorithm>
#include <numeric>
using namespace openwbo;

struct less_than_wlitt {
    inline bool operator()(const wlitt &wl1, const wlitt &wl2) const {
        return (wl1.weight < wl2.weight);
    }
};

Lit GTE::getNewLit(Solver *S) {
    Lit p = mkLit(S->nVars(), false);
    newSATVariable(S);
    nb_variables++;
    return p;
}

Lit GTE::get_var(Solver *S, wlit_mapt &oliterals, uint64_t weight) {
    wlit_mapt::iterator it = oliterals.find(weight);
    if (it == oliterals.end()) {
        Lit v = getNewLit(S);
        oliterals[weight] = v;
    }
    return oliterals[weight];
}

bool GTE::encodeLeq(uint64_t k, Solver *S, const weightedlitst &iliterals,
                    wlit_mapt &oliterals, uint64_t iliterals_start,
                    uint64_t iliterals_end) {

    uint64_t iliterals_size = iliterals_end - iliterals_start;
    if (iliterals_size == 0 || k == 0)
        return false;

    if (iliterals_size == 1) {
        oliterals.insert(wlit_pairt(iliterals[iliterals_start].weight,
                                    iliterals[iliterals_start].lit));
        return true;
    }

    wlit_mapt loutputs, routputs;

    unsigned int lsize = iliterals_size >> 1;

    if (gte_alg == _GTE_CLUSTER_) {
        int i = lsize - 1;
        for (; i >= 0; i--) {
            if (iliterals[i].weight != iliterals[lsize].weight) {
                break;
            }
        }
        if (i == -1) {
            i = lsize;
            for (; i < iliterals_size; i++) {
                if (iliterals[i].weight != iliterals[lsize].weight) {
                    break;
                }
            }
            if (i != iliterals_size) {
                lsize = i;
            } else {
                lsize = iliterals_size >> 1;
            }
        } else {
            lsize = i + 1;
        }
    }

    weightedlitst::const_iterator myit = iliterals.begin() + iliterals_start;
    weightedlitst::const_iterator myit1 = myit + lsize;
    weightedlitst::const_iterator myit2 = iliterals.begin() + iliterals_end;

    wlit_sumt wlit_sum;
    uint64_t lk = std::accumulate(myit, myit1, uint64_t(0), wlit_sum);
    uint64_t rk = std::accumulate(myit1, myit2, uint64_t(0), wlit_sum);

    lk = k >= lk ? lk : k;
    rk = k >= rk ? rk : k;

    bool result = encodeLeq(lk, S, iliterals, loutputs, iliterals_start,
                            iliterals_start + lsize);
    if (!result)
        return result;
    result = result && encodeLeq(rk, S, iliterals, routputs,
                                 iliterals_start + lsize, iliterals_end);
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
                addBinaryClause(S, ~mit->second,
                                get_var(S, oliterals, mit->first));
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
                addBinaryClause(S, ~mit->second,
                                get_var(S, oliterals, mit->first));
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
            for (wlit_mapt::iterator rit = routputs.begin();
                 rit != routputs.end(); rit++) {
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

void GTE::encode(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs,
                 uint64_t rhs) {
    // FIXME: do not change coeffs in this method. Make coeffs const.

    // If the rhs is larger than INT64_MAX is not feasible to encode this
    // pseudo-Boolean constraint to CNF.
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

    if (lits.size() == 1) {
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

    encodeLeq(rhs + 1, S, iliterals, pb_oliterals, 0, iliterals.size());

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

void GTE::update(Solver *S, uint64_t rhs) {

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
    /* ... PUT CODE HERE TO UPDATE THE RHS OF AN ALREADY EXISTING ENCODING ...
     */

    current_pb_rhs = rhs;
}

// predict number of variables and clauses that this encode will generate
Lit GTE::get_var_predict(Solver *S, wlit_mapt &oliterals, uint64_t weight) {
    wlit_mapt::iterator it = oliterals.find(weight);
    if (it == oliterals.end()) {
        Lit v = mkLit(nb_current_variables, false);
        nb_current_variables++;
        oliterals[weight] = v;
    }
    return oliterals[weight];
}

bool GTE::predictEncodeLeq(uint64_t k, Solver *S,
                           const weightedlitst &iliterals, wlit_mapt &oliterals,
                           uint64_t iliterals_start, uint64_t iliterals_end) {

    if (nb_clauses_expected >= MAX_CLAUSES)
        return false;

    uint64_t iliterals_size = iliterals_end - iliterals_start;
    if (iliterals_size == 0 || k == 0)
        return false;

    if (iliterals_size == 1) {
        oliterals.insert(wlit_pairt(iliterals[iliterals_start].weight,
                                    iliterals[iliterals_start].lit));
        return true;
    }

    wlit_mapt loutputs, routputs;

    unsigned int lsize = iliterals_size >> 1;
    weightedlitst::const_iterator myit = iliterals.begin() + iliterals_start;
    weightedlitst::const_iterator myit1 = myit + lsize;
    weightedlitst::const_iterator myit2 = iliterals.begin() + iliterals_end;

    wlit_sumt wlit_sum;
    uint64_t lk = std::accumulate(myit, myit1, uint64_t(0), wlit_sum);
    uint64_t rk = std::accumulate(myit1, myit2, uint64_t(0), wlit_sum);

    lk = k >= lk ? lk : k;
    rk = k >= rk ? rk : k;

    bool result = predictEncodeLeq(lk, S, iliterals, loutputs, iliterals_start,
                                   iliterals_start + lsize);
    if (!result)
        return result;
    result = result && predictEncodeLeq(rk, S, iliterals, routputs,
                                        iliterals_start + lsize, iliterals_end);
    if (!result)
        return result;

    {
        assert(!loutputs.empty());
        for (wlit_mapt::iterator mit = loutputs.begin(); mit != loutputs.end();
             mit++) {
            if (mit->first > k) {
                get_var_predict(S, oliterals, k);
                nb_clauses_expected++;
                if (nb_clauses_expected >= MAX_CLAUSES)
                    return false;
            } else {
                get_var_predict(S, oliterals, mit->first);
                nb_clauses_expected++;
                if (nb_clauses_expected >= MAX_CLAUSES)
                    return false;
            }
        }
    }

    {
        assert(!routputs.empty());
        for (wlit_mapt::iterator mit = routputs.begin(); mit != routputs.end();
             mit++) {
            if (mit->first > k) {
                get_var_predict(S, oliterals, k);
                nb_clauses_expected++;
                if (nb_clauses_expected >= MAX_CLAUSES)
                    return false;
            } else {
                get_var_predict(S, oliterals, mit->first);
                nb_clauses_expected++;
                if (nb_clauses_expected >= MAX_CLAUSES)
                    return false;
            }
        }
    }

    {
        for (wlit_mapt::iterator lit = loutputs.begin(); lit != loutputs.end();
             lit++) {
            for (wlit_mapt::iterator rit = routputs.begin();
                 rit != routputs.end(); rit++) {
                uint64_t tw = lit->first + rit->first;
                if (tw > k) {
                    get_var_predict(S, oliterals, k);
                    nb_clauses_expected++;
                    if (nb_clauses_expected >= MAX_CLAUSES)
                        return false;
                } else {
                    get_var_predict(S, oliterals, tw);
                    nb_clauses_expected++;
                    if (nb_clauses_expected >= MAX_CLAUSES)
                        return false;
                }
            }
        }
    }

    return true;
}

int GTE::predict(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs,
                 uint64_t rhs) {

    vec<Lit> simp_lits;
    vec<uint64_t> simp_coeffs;
    lits.copyTo(simp_lits);
    coeffs.copyTo(simp_coeffs);
    lits.clear();
    coeffs.clear();

    nb_current_variables = S->nVars();

    for (int i = 0; i < simp_lits.size(); i++) {
        if (simp_coeffs[i] == 0)
            continue;

        if (simp_coeffs[i] >= INT64_MAX) {
            return MAX_CLAUSES;
        }

        if (simp_coeffs[i] <= (unsigned)rhs) {
            lits.push(simp_lits[i]);
            coeffs.push(simp_coeffs[i]);
        } else
            nb_clauses_expected++;
    }

    if (lits.size() <= 1)
        return nb_clauses_expected;

    weightedlitst iliterals;
    for (int i = 0; i < lits.size(); i++) {
        wlitt wl;
        wl.lit = lits[i];
        wl.weight = coeffs[i];
        iliterals.push_back(wl);
    }
    less_than_wlitt lt_wlit;
    std::sort(iliterals.begin(), iliterals.end(), lt_wlit);
    wlit_mapt oliterals;
    predictEncodeLeq(rhs + 1, S, iliterals, oliterals, 0, iliterals.size());
    return nb_clauses_expected;
}
