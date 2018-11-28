# Open-WBO-Inc with SATLike
[![Build Status](https://travis-ci.com/GoodDeeds/open-wbo-incomplete.svg?token=mYsgPy4zsL5qQDoHBaME&branch=satlike)](https://travis-ci.com/GoodDeeds/open-wbo-incomplete)

This branch provides a hybrid version of Open-WBO-Inc-BMO [2,3] and SATLike [4]. The main README file for Open-WBO-Inc can be found in the master branch.

Open-WBO-Inc-BMO uses a subproblem minimization based approximation strategy to speedup MaxSAT solving. However, the optimal solution of this algorithm need not be optimal. If time is available after this output is found, the solver could switch to another algorithm and attempt to find a better solution. Here, we provide a hybrid of Open-WBO-Inc-BMO and SATLike, called inc-bmo-satlike, in which the solver switches to SATLike by initializing it with the best solution found by Open-WBO-Inc-BMO. The hybrid's behaviour is identical to Open-WBO-Inc-BMO for benchmarks where the solver does not terminate with the optimal of Open-WBO-Inc-BMO within the time limit.

## Installation, General Usage and Data Formats
Please refer to the README in the master branch.

## Running inc-bmo-satlike

The Open-WBO-Inc-BMO component uses linear search with clustering under the BMO assumption. Weights are divided into 100000 clusters (and can be modified appropriately by changing the value passed to the `c` flag). The optimal found (if any) by Open-WBO-Inc-BMO is used to initialize SATLike. To run, use
```bash
$ ./open-wbo-inc -no-complete -ca=1 -c=100000 -algorithm=6 <input-file>
```

## License and Copyright
The licenses and copyright notices for Open-WBO-Inc and its dependencies can be found [here](LICENSE).
The license for SATLike can be found [here](LICENSE.satlike).

## References
1. Ruben Martins, Vasco Manquinho, Inês Lynce. (2014) Open-WBO: A Modular MaxSAT Solver,. In: Sinz C., Egly U. (eds) Theory and Applications of Satisfiability Testing – SAT 2014. SAT 2014. Lecture Notes in Computer Science, vol 8561. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007/978-3-319-09284-3_33)]
2. Saurabh Joshi, Prateek Kumar, Ruben Martins, Sukrut Rao. (2018) Approximation Strategies for Incomplete MaxSAT. In: Hooker J. (eds) Principles and Practice of Constraint Programming. CP 2018. Lecture Notes in Computer Science, vol 11008. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007%2F978-3-319-98334-9_15)]
3. Saurabh Joshi, Prateek Kumar, Vasco Manquinho, Ruben Martins, Alexander Nadel and Sukrut Rao. Open-WBO-Inc in MaxSAT Evaluation 2018. MaxSAT Evaluation 2018, p.16. [[Link](https://helda.helsinki.fi/bitstream/handle/10138/237139/mse18_proceedings.pdf?sequence=1#page=17)]
4. Zhendong Lei and Shaowei Cai. SATLike: Solver Description. MaxSAT Evaluation 2018, p.23. [[Link](https://helda.helsinki.fi/bitstream/handle/10138/237139/mse18_proceedings.pdf?sequence=1#page=24)]
