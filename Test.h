#include <random>
#include <algorithm>
#include <vector>
#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "encodings/Enc_GTECluster.h"
#include "encodings/Enc_GTEIncremental.h"

#define NUM_CLUSTERS 10
#define MAX_PER_CLUSTER 20

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
	
	for(int i = 0; i < assumptions.size(); i++) {
		std::cout << var(assumptions[i]) << std::endl;
	}

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
}

void test_encoding()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<unsigned int> dis(1, MAX_PER_CLUSTER);
	std::uniform_int_distribution<uint64_t> dis64(1, 1000);

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

	std::uniform_int_distribution<unsigned> dis_unit(1, weights.size()/4);
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
		std::uniform_int_distribution<unsigned> dis_rhs(sum/2-1, sum-1);
		rhs = dis_rhs(g);
	} else {
		std::uniform_int_distribution<unsigned> dis_rhs(sum, 2*sum+1);
		rhs = dis_rhs(g);
	}

	std::cout << "Number of lits: " << literals.size() << std::endl
		<< "Number of unit clauses: " << num_unit_clauses << std::endl;
	std::cout << "RHS: " << rhs << std::endl;

	gte.encode(s, literals, weights_vec, rhs);
	gte.update(s, rhs, assumptions);

	std::cout << "Encoded" << std::endl;
	//return;

	bool solved = s->solve(assumptions);

	if (unsat) {
		if (solved) {
			std::cout << "TEST FAILED" << std::endl;
			std::cout << "SAT" << std::endl;
			std::cout << "c RHS " << rhs << std::endl;
			std::cout << "p wcnf " << weights_vec.size() << " " << weights_vec.size()+num_unit_clauses
				<< " " << 1500 << std::endl;
			for (int i=0; i<weights_vec.size(); i++) {
				std::cout << weights_vec[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<num_unit_clauses; i++) {
				std::cout << "1500 " << i+1 << " 0" << std::endl;
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
				<< " " << 1500 << std::endl;
			for (int i=0; i<weights_vec.size(); i++) {
				std::cout << weights_vec[i] << " " << i+1 << " 0" << std::endl;
			}
			for (int i=0; i<num_unit_clauses; i++) {
				std::cout << "1500 " << i+1 << " 0" << std::endl;
			}
			std::cout << "c DONE" << std::endl;
		}
	}

}

#endif
