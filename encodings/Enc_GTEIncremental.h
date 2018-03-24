#ifndef Enc_GTEIncremental_h
#define Enc_GTEIncremental_h

#ifdef SIMP
#include "simp/SimpSolver.h"
#else
#include "core/Solver.h"
#endif

#include "Encodings.h"
#include "core/SolverTypes.h"
#include "../MaxTypes.h"
#include <map>
#include <utility>
#include <vector>

#include "Enc_GTE.h"

namespace openwbo {

class GTEIncremental : public Encodings {

public:
  GTEIncremental(int strategy = _INCREMENTAL_NONE_) {
    // current_pb_rhs = -1; // -1 corresponds to an unitialized value
    current_pb_rhs = 0;
    nb_clauses = 0;
    nb_variables = 0;
    incremental_strategy = strategy;
  }
  ~GTEIncremental() {}

  // Encode constraint.
  void encode(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs, uint64_t rhs);
  void build(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs, uint64_t rhs);

  // Update constraint.
  void update(Solver *S, uint64_t rhs, vec<Lit> &assumptions);

  // Returns true if the encoding was built, otherwise returns false;
  bool hasCreatedEncoding() { return hasEncoding; }
  
  // Joins two trees in iterative encoding
  void join(Solver *S, vec<Lit> &lits, vec<uint64_t> &coeffs, uint64_t rhs, 
  		vec<Lit> &assumptions);

protected:
  void printLit(Lit l) { printf("%s%d\n", sign(l) ? "-" : "", var(l) + 1); }

  bool encodeLeq(uint64_t k, Solver *S, const weightedlitst &iliterals,
                 wlit_mapt &oliterals);
  // encodeLeq for iterative encoding     
  bool encodeLeqIncremental(uint64_t k, Solver *S,
  				const weightedlitst &iliterals, 
  				wlit_mapt &oliterals, uint64_t old_k = 0);
  Lit getNewLit(Solver *S);
  Lit get_var(Solver *S, wlit_mapt &oliterals, uint64_t weight);
  vec<Lit> pb_outlits; // Stores the outputs of the pseudo-Boolean constraint
                       // encoding for incremental solving.
  uint64_t current_pb_rhs; // Stores the current value of the rhs of the
                           // pseudo-Boolean constraint.

  // Stores unit lits. Used for lits that have a coeff larger than rhs.
  wlit_mapt pb_oliterals;
  vec<Lit> unit_lits;
  vec<uint64_t> unit_coeffs;

  // Number of variables and clauses for statistics.
  int nb_variables;
  int nb_clauses;
  
  int incremental_strategy;
  weightedlitst enc_literals;
};

} // namespace openwbo

#endif
