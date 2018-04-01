#ifndef TEST_H
#define TEST_H

#include <random>
#include <algorithm>
#include <vector>

#include <iostream>
#include "encodings/Enc_GTECluster.h"
#include "encodings/Enc_GTEIncremental.h"

#define NUM_CLUSTERS 3
#define MAX_PER_CLUSTER 4
#define MAX_WEIGHT 10

void test_encoding(MaxSATFormula *maxsat_formula, uint64_t rhs1, uint64_t rhs2, int nsoft1) {
	Solver *s = new Solver();

	vec<Lit> assumptions;
	vec<Lit> literals1;
	vec<uint64_t> weights_vec1;
	vec<Lit> literals2;
	vec<uint64_t> weights_vec2;

	for (int i=0; i<maxsat_formula->nSoft(); i++) {
		s->newVar();
		if (i<nsoft1) {
			literals1.push(maxsat_formula->getSoftClause(i).clause[0]);
			weights_vec1.push(maxsat_formula->getSoftClause(i).weight);
		} else {
			literals2.push(maxsat_formula->getSoftClause(i).clause[0]);
			weights_vec2.push(maxsat_formula->getSoftClause(i).weight);
		}
	}

	openwbo::GTEIncremental gte(_INCREMENTAL_ITERATIVE_);
	gte.encode(s, literals1, weights_vec1, rhs1);
	gte.update(s, rhs1, assumptions);
	gte.join(s, literals2, weights_vec2, rhs2, assumptions);
	gte.update(s, rhs2, assumptions);

	for (int i=0; i<maxsat_formula->nHard(); i++) {
		s->addClause(maxsat_formula->getHardClause(i).clause[0]);
	}

	bool solved = s->solve(assumptions);
	if (solved) {
		std::cout << "SAT" << std::endl;
	} else {
		std::cout << "UNSAT" << std::endl;
	}
	
	delete s;
}

void test_encoding_join()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<unsigned int> dis(1, MAX_PER_CLUSTER);
	std::uniform_int_distribution<uint64_t> dis64(1, MAX_WEIGHT);

	std::vector<uint64_t> weights;
	for (int i=0; i<NUM_CLUSTERS; i++) {
		int n = dis(g);
		uint64_t w = dis64(g);
		for (int j=0; j<n; j++) {
			weights.push_back(w);
		}
	}

	Solver *s = new Solver();

	std::vector<Lit> literals_vector;
	vec<Lit> literals1;
	vec<Lit> literals2;
	vec<uint64_t> weights_vec;
	vec<uint64_t> weights_vec1;
	vec<uint64_t> weights_vec2;

	for (unsigned i=0; i<weights.size(); i++) {
		literals_vector.push_back(mkLit(s->newVar(), false));
		weights_vec.push(weights[i]);
	}

	std::shuffle(literals_vector.begin(), literals_vector.end(), g);

	std::uniform_int_distribution<unsigned> dis_tree2(0,1);

	for (unsigned i=0; i<literals_vector.size(); i++) {
		unsigned take_tree2 = dis_tree2(g);
		if (take_tree2) {
			literals2.push(literals_vector[i]);
			weights_vec2.push(weights_vec[i]);
		}
		else {
			literals1.push(literals_vector[i]);
			weights_vec1.push(weights_vec[i]);
		}
	}

	std::uniform_int_distribution<unsigned> dis_unit(1, weights.size()/4+1);
	unsigned num_unit_clauses = dis_unit(g);

	uint64_t sum = 0;
	for (unsigned i=0; i<num_unit_clauses; i++) {
		sum += weights[i];
		s->addClause(literals_vector[i]);
	}

	openwbo::GTEIncremental gte(_INCREMENTAL_ITERATIVE_);
	vec<Lit> assumptions;
	uint64_t rhs = 0;
	std::uniform_int_distribution<unsigned> dis_rhs(0,1);
	unsigned unsat = dis_rhs(g);
	if (unsat) {
		std::uniform_int_distribution<unsigned> dis_rhs(0, sum-1);
		rhs = dis_rhs(g);
	} else {
		std::uniform_int_distribution<unsigned> dis_rhs(sum, 2*sum+1);
		rhs = dis_rhs(g);
	}

	std::uniform_int_distribution<unsigned> dis_rhs1(0, 2*sum+1);
	uint64_t rhs1 = dis_rhs1(g);

	std::cout << "Number of lits1: " << literals1.size() << std::endl
		<< "Number of lits2: " << literals2.size() << std::endl
	 	<< "Number of unit clauses: " << num_unit_clauses << std::endl;
	std::cout << "RHS: " << rhs << std::endl;

	gte.encode(s, literals1, weights_vec1, rhs1);
	gte.update(s, rhs1, assumptions);
	gte.join(s, literals2, weights_vec2, rhs, assumptions);
	gte.update(s, rhs, assumptions);

	std::cout << "Encoded" << std::endl;

	bool solved = s->solve(assumptions);

	if (unsat) {
		if (solved) {
			std::cout << "TEST FAILED" << std::endl;
			std::cout << "SAT" << std::endl;
			std::cout << "c RHS1 " << rhs1 << std::endl;
			std::cout << "c RHS2 " << rhs << std::endl;
			std::cout << "c Nsoft " << weights_vec1.size() << std::endl;
			std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
				<< " " << MAX_WEIGHT+5 << std::endl;
			for (int i=0; i<weights_vec1.size(); i++) {
				std::cout << weights_vec1[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<weights_vec2.size(); i++) {
				std::cout << weights_vec2[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<num_unit_clauses; i++) {
				std::cout << MAX_WEIGHT+5 << " " << i+1 << " 0" << std::endl;
			}
			std::cout << "c DONE" << std::endl;
		} else {
			std::cout << "UNSAT" << std::endl;
		}
	} else {
		if (solved) {
			std::cout << "SAT" << std::endl;
		} else {
			std::cout << "TEST FAILED" << std::endl;
			std::cout << "UNSAT" << std::endl;
			std::cout << "c RHS1 " << rhs1 << std::endl;
			std::cout << "c RHS2 " << rhs << std::endl;
			std::cout << "c Nsoft " << weights_vec1.size() << std::endl;
			std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
				<< " " << MAX_WEIGHT+5 << std::endl;
			for (int i=0; i<weights_vec1.size(); i++) {
				std::cout << weights_vec1[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<weights_vec2.size(); i++) {
				std::cout << weights_vec2[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<num_unit_clauses; i++) {
				std::cout << MAX_WEIGHT+5 << " " << i+1 << " 0" << std::endl;
			}
			std::cout << "c DONE" << std::endl;
		}
	}

	delete s;
}

void test_encoding(MaxSATFormula *maxsat_formula, uint64_t rhs) {
	Solver *s = new Solver();

	openwbo::GTEIncremental gte(_INCREMENTAL_ITERATIVE_);
	vec<Lit> assumptions;
	vec<Lit> literals;
	vec<uint64_t> weights_vec;

	for (int i=0; i<maxsat_formula->nSoft(); i++) {
		s->newVar();
		literals.push(maxsat_formula->getSoftClause(i).clause[0]);
		weights_vec.push(maxsat_formula->getSoftClause(i).weight);
	}

	gte.encode(s, literals, weights_vec, rhs);
	gte.update(s, rhs, assumptions);
	
//	for(int i = 0; i < assumptions.size(); i++) {
//		std::cout << var(assumptions[i]) << std::endl;
//	}

	std::cout << "Encoded" << std::endl;

	for (int i=0; i<maxsat_formula->nHard(); i++) {
		s->addClause(maxsat_formula->getHardClause(i).clause[0]);
	}

	bool solved = s->solve(assumptions);
	if (solved) {
		std::cout << "SAT" << std::endl;
	} else {
		std::cout << "UNSAT" << std::endl;
	}
//	std::cout << "Inc" << std::endl;
//	while(rhs > 0) {
//		std::cin >> rhs;
//		if(rhs <= 0) break;
//		gte.update(s, rhs, assumptions);
//		bool solved = s->solve(assumptions);
//		if (solved) {
//			std::cout << "SAT" << std::endl;
//		} else {
//			std::cout << "UNSAT" << std::endl;
//		}
//	}
	
	delete s;
}

void test_encoding()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<unsigned int> dis(1, MAX_PER_CLUSTER);
	std::uniform_int_distribution<uint64_t> dis64(1, MAX_WEIGHT);

	std::vector<uint64_t> weights;
	for (int i=0; i<NUM_CLUSTERS; i++) {
		int n = dis(g);
		uint64_t w = dis64(g);
		for (int j=0; j<n; j++) {
			weights.push_back(w);
		}
	}

	Solver *s = new Solver();

	std::vector<Lit> literals_vector;
	vec<Lit> literals;
	vec<uint64_t> weights_vec;

	// std::shuffle(weights.begin(), weights.end(), g);
	for (unsigned i=0; i<weights.size(); i++) {
		literals_vector.push_back(mkLit(s->newVar(), false));
		weights_vec.push(weights[i]);
	}

	std::shuffle(literals_vector.begin(), literals_vector.end(), g);

	for (unsigned i=0; i<literals_vector.size(); i++) {
		literals.push(literals_vector[i]);
	}

	std::uniform_int_distribution<unsigned> dis_unit(1, weights.size()/4+1);
	unsigned num_unit_clauses = dis_unit(g);

	uint64_t sum = 0;
	for (unsigned i=0; i<num_unit_clauses; i++) {
		sum += weights[i];
		s->addClause(literals[i]);
	}

	openwbo::GTEIncremental gte(_INCREMENTAL_ITERATIVE_);
	vec<Lit> assumptions;
	uint64_t rhs = 0;
	std::uniform_int_distribution<unsigned> dis_rhs(0,1);
	unsigned unsat = dis_rhs(g);
	if (unsat) {
		std::uniform_int_distribution<unsigned> dis_rhs(0, sum-1);
		rhs = dis_rhs(g);
	} else {
		std::uniform_int_distribution<unsigned> dis_rhs(sum, 2*sum+1);
		rhs = dis_rhs(g);
	}

	std::cout << "Number of lits: " << literals.size() << std::endl
	 	<< "Number of unit clauses: " << num_unit_clauses << std::endl;
	std::cout << "RHS: " << rhs << std::endl;
	
//	std::cout << "c RHS " << rhs << std::endl;
//	std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
//		<< " " << MAX_WEIGHT+5 << std::endl;
//	for (int i=0; i<weights_vec.size(); i++) {
//		std::cout << weights_vec[i] << " " << i+1 << " 0" << std::endl;
//	}
//	for (int i=0; i<num_unit_clauses; i++) {
//		std::cout << MAX_WEIGHT+5 << " " << i+1 << " 0" << std::endl;
//	}
//	std::cout << "c DONE" << std::endl; 

	gte.encode(s, literals, weights_vec, rhs);
	
	std::uniform_int_distribution<unsigned> dis_num_inc(1, 100);
	unsigned num_inc = dis_num_inc(g);
	
	for (int k=0; k < num_inc; k++) {
//		std::cin >> rhs;
//		if(rhs < sum) {
//			unsat = true;
//		} else {
//			unsat = false;
//		}
		std::cout << "RHS : " << rhs << std::endl;
		gte.update(s, rhs, assumptions);

		std::cout << "Encoded" << std::endl;

		bool solved = s->solve(assumptions);

		if (unsat) {
			if (solved) {
				std::cout << "TEST FAILED" << std::endl;
				std::cout << "SAT" << std::endl;
				std::cout << "c RHS " << rhs << std::endl;
				std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
					<< " " << MAX_WEIGHT+5 << std::endl;
				for (int i=0; i<weights_vec.size(); i++) {
					std::cout << weights_vec[i] << " " << i+1 << " 0" << std::endl;
				}
				for (int i=0; i<num_unit_clauses; i++) {
					std::cout << MAX_WEIGHT+5 << " " << i+1 << " 0" << std::endl;
				}
				std::cout << "c DONE" << std::endl;
			} else {
				std::cout << "UNSAT" << std::endl;
			}
		} else {
			if (solved) {
				std::cout << "SAT" << std::endl;
			} else {
				std::cout << "TEST FAILED" << std::endl;
				std::cout << "UNSAT" << std::endl;
				std::cout << "c RHS " << rhs << std::endl;
				std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
					<< " " << MAX_WEIGHT+5 << std::endl;
				for (int i=0; i<weights_vec.size(); i++) {
					std::cout << weights_vec[i] << " " << i+1 << " 0" << std::endl;
				}
				for (int i=0; i<num_unit_clauses; i++) {
					std::cout << MAX_WEIGHT+5 << " " << i+1 << " 0" << std::endl;
				}
				std::cout << "c DONE" << std::endl;
			}
		}

		std::uniform_int_distribution<unsigned> dis_rhs(0,1);
		unsat = dis_rhs(g);
		if (unsat) {
			std::uniform_int_distribution<unsigned> dis_rhs(0, sum-1);
			rhs = dis_rhs(g);
		} else {
			std::uniform_int_distribution<unsigned> dis_rhs(sum, 2*sum+1);
			rhs = dis_rhs(g);
		}
	}
	
	delete s;
}

#endif
