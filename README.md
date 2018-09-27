# Open-WBO-Inc Incomplete MaxSAT Solver

Open-WBO-Inc is a partial incomplete MaxSAT solver built on top of [Open-WBO](https://github.com/sat-group/open-wbo) [1], an extensible and modular open-source MaxSAT solver that has been one of the best solvers in the partial MaxSAT categories at the [MaxSAT Evaluations](https://maxsat-evaluations.github.io/) [2014](http://www.maxsat.udl.cat/14/), [2015](http://www.maxsat.udl.cat/15/index.html), [2016](http://www.maxsat.udl.cat/16/index.html), and [2017](https://maxsat-evaluations.github.io/2017/), and in the decision and optimization for SMALLINT categories at the [PB Evaluation 2016](http://www.cril.univ-artois.fr/PB16/).

Open-WBO-Inc helps in finding quick approximate solutions to MaxSAT problems by using weight relaxation strategies as proposed in [2]. A version of Open-WBO-Inc implementing the apx-subprob algorithm proposed in the paper secured the first and second positions in the 60s and 300s categories respectively at the [MaxSAT Evaluations 2018](https://maxsat-evaluations.github.io/2018/). Another version implementing the apx-weight algorithm secured the fourth position in both these categories.

Open-WBO-Inc contains all the features of Open-WBO 2.0, with extensions. Further details about the version submitted to the MaxSAT Evaluations 2018 can be found in [3].

## Installation
The installation procedure is the same as Open-WBO 2.0, and can be found in [INSTALL.md](INSTALL.md).

## Usage
Open-WBO-Inc can be used in the same way as Open-WBO. A description of the new features added in Open-WBO-Inc for incomplete MaxSAT solving using the apx-weight and apx-subprob algorithms is provided below.

TODO

## Authors and Contributors
### Authors
* [Saurabh Joshi](https://sbjoshi.github.io/)
* [Prateek Kumar](https://prateekkumar.in/)
* [Ruben Martins](https://sat-group.github.io/ruben/)
* [Sukrut Rao](https://github.com/GoodDeeds/)
### Contributors
* [Alexander Nadel](http://www.cs.tau.ac.il/research/alexander.nadel/)
* [Vasco Manquinho](http://sat.inesc-id.pt/~vmm/)

To contact the authors, please send an email to:  [open-wbo@sat.inesc-id.pt](mailto:open-wbo@sat.inesc-id.pt)

## License and Copyright
The license and copyright notice for this software and its dependencies can be found [here](LICENSE).

## References
1. Ruben Martins, Vasco Manquinho, Inês Lynce. (2014) Open-WBO: A Modular MaxSAT Solver,. In: Sinz C., Egly U. (eds) Theory and Applications of Satisfiability Testing – SAT 2014. SAT 2014. Lecture Notes in Computer Science, vol 8561. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007/978-3-319-09284-3_33)]
2. Saurabh Joshi, Prateek Kumar, Ruben Martins, Sukrut Rao. (2018) Approximation Strategies for Incomplete MaxSAT. In: Hooker J. (eds) Principles and Practice of Constraint Programming. CP 2018. Lecture Notes in Computer Science, vol 11008. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007%2F978-3-319-98334-9_15)]
3. Saurabh Joshi, Prateek Kumar, Vasco Manquinho, Ruben Martins, Alexander Nadel and Sukrut Rao. Open-WBO-Inc in MaxSAT Evaluation 2018. MaxSAT Evaluation 2018, p.16. [[Link](https://helda.helsinki.fi/bitstream/handle/10138/237139/mse18_proceedings.pdf?sequence=1#page=17)]
4. Alexander Nadel. (2018) Solving MaxSAT with Bit-Vector Optimization. In: Beyersdorff O., Wintersteiger C. (eds) Theory and Applications of Satisfiability Testing – SAT 2018. SAT 2018. Lecture Notes in Computer Science, vol 10929. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007/978-3-319-94144-8_4)]
