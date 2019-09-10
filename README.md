# Open-WBO-Inc Incomplete MaxSAT Solver
[![Build Status](https://travis-ci.com/sukrutrao/open-wbo-incomplete.svg?token=mYsgPy4zsL5qQDoHBaME&branch=master)](https://travis-ci.com/sukrutrao/open-wbo-incomplete)

Open-WBO-Inc is a partial incomplete MaxSAT solver built on top of [Open-WBO](https://github.com/sat-group/open-wbo) [1]. Open-WBO is an extensible and modular open-source MaxSAT solver that has been one of the best solvers in the partial MaxSAT categories at the [MaxSAT Evaluations](https://maxsat-evaluations.github.io/) [2014](http://www.maxsat.udl.cat/14/), [2015](http://www.maxsat.udl.cat/15/index.html), [2016](http://www.maxsat.udl.cat/16/index.html), and [2017](https://maxsat-evaluations.github.io/2017/), and in the decision and optimization for SMALLINT categories at the [PB Evaluation 2016](http://www.cril.univ-artois.fr/PB16/).

Open-WBO-Inc helps in finding quick approximate solutions to MaxSAT problems by using weight relaxation strategies as proposed in [2]. A version of Open-WBO-Inc implementing the apx-subprob algorithm proposed in the paper secured the first and second positions in the 60s and 300s categories respectively at the [MaxSAT Evaluations 2018](https://maxsat-evaluations.github.io/2018/). Another version implementing the apx-weight algorithm secured the fourth position in both these categories.

Open-WBO-Inc contains all the features of Open-WBO 2.0, with extensions. Further details about the version submitted to the MaxSAT Evaluations 2018 can be found in [3].

## Installation
The installation procedure is the same as Open-WBO 2.0, and can be found [here](INSTALL.md).

## Usage
Open-WBO-Inc can be used in the same way as Open-WBO. Instructions on using the solver, specific algorithms, and a description of the input and output formats are provided in this section.

To run the solver on a MaxSAT problem, use
```
$ ./open-wbo-inc [options] <input-file>
```
where `<input-file>` contains the MaxSAT formula.

To obtain an overview of the options available, use
```
$ ./open-wbo-inc --help
```
For verbose help, use
```
$ ./open-wbo-inc --help-verb
```

### Input Format
The input must be provided in the [standard WCNF format](https://maxsat-evaluations.github.io/2018/rules.html#input), and must follow the restrictions on the weights specified.

### Output Format
The output is provided in [this format](https://maxsat-evaluations.github.io/2018/rules.html#output).

The output consists of three types of lines, characterized by the first character on the line. They are:
* Comments ("c" lines)

  These lines are comments. The start with the character "c", followed by a space, and then followed by any text.
* Solution cost ("o" lines)

  The cost is the sum of weights of unsatisfied clauses in a given assignment. The goal of MaxSAT solving is to find an assignment that minimizes the cost. For incomplete MaxSAT solving, Open-WBO-Inc tries to find an assignment that approximates the actual minimum cost.

  Open-WBO-Inc outputs a new cost line each time it finds a better assignment that improves upon the previous best cost. This consists of the character "o", followed by a space, and then followed by the cost found. At any point of time, the last solution cost line printed represents the best cost found so far by the solver.
* Solution status ("s" line)

  This line is displayed once at the end when the solver terminates, either because the best solution has been found or because it has received a SIGTERM signal. This indicates the nature of the solution found by the solver. It consists of the character "s", followed by a space, and then followed by the status, which can be of four types:
  * OPTIMUM FOUND:
    This indicates that the solver has found exactly the least possible cost for the formula.
  * SATISFIABLE:
    This indicates that the solver has found some assignment that satisfies the formula and its associated cost, but it is not known whether the assignment is optimal. This could be because of the presence of a time limit, exceeding which solver received a SIGTERM signal to terminate. The best solution obtained so far is provided.
  * UNSATISFIABLE:
    This indicates that the hard clauses in the formula could not be satisfied by the solver.
  * UNKNOWN:
    This indicates that the solver was unable to solve the problem, which could be due to various reasons. Examples include the case where the solver was unable to find any solution within the time and memory limits, or where the path of the input file provided was invalid.
* Solution Values (Truth Assignment) ("v" lines)

  This provides the assignment for the model found by the solver. It consists of the character "v", followed by a space, and then followed by the assignment. The assignment consists of contiguous space separated integers in increasing order, the absolute value of which corresponds to the variable it represents in the input. If the integer is displayed with a negative sign, the variable has been assigned false, and if not, the variable has been assigned true.

### Running specific algorithms
This section provides instructions to run specific incomplete MaxSAT algorithms.

#### Weighted

* Open-WBO-Inc-BMO [3] \(apx-subprob [2]\)

  This uses linear search with clustering under the BMO assumption. Weights are divided into 100000 clusters. To run, use
  ```
  $ ./open-wbo-inc -no-complete -ca=1 -c=100000 -algorithm=6 <input-file>
  ```
* Open-WBO-Inc-Cluster [3] \(apx-weight [2]\)

  This uses linear search with clustering. Weights are divided into two clusters. To run, use
  ```
  $ ./open-wbo-inc -ca=1 -c=2 -cardinality=2 -pb=1 -algorithm=1 <input-file>
  ```

* inc-bmo-complete
  
  This runs Open-WBO-Inc-BMO first, and if an optimal solution is found under the BMO assumption, it switches to the LSU algorithm in order to search for a better bound. To run, use
    ```
  $ ./open-wbo-inc -ca=1 -c=100000 -algorithm=6 <input-file>
  ```

#### Unweighted

* Open-WBO-Inc-MCS [3]

  To run, use
  ```
  $ ./open-wbo-inc -cardinality=2 -conflicts=100000 -iterations=30 -algorithm=8 <input-file>
  ```
* Open-WBO-Inc-OBV \[3\]\[4\]

  To run, use
  ```
  $ ./open-wbo-inc -cardinality=2 -conflicts=10000 -iterations=100 -algorithm=7 <input-file>
  ```

## Authors and Contributors
### Authors
* [Saurabh Joshi](https://sbjoshi.github.io/)
* [Prateek Kumar](https://prateekkumar.in/)
* [Ruben Martins](https://sat-group.github.io/ruben/)
* [Sukrut Rao](https://sukrutrao.github.io/)

### Contributors
* [Vasco Manquinho](http://sat.inesc-id.pt/~vmm/)
* [Alexander Nadel](http://www.cs.tau.ac.il/research/alexander.nadel/)

To contact the authors, please send an email to:  [open-wbo@sat.inesc-id.pt](mailto:open-wbo@sat.inesc-id.pt)

## License and Copyright
The licenses and copyright notices for this software and its dependencies can be found [here](LICENSE).

## Citation
If Open-WBO-Inc is useful for your research, we request you to cite the papers that describe the components (algorithms, encodings, etc.) that you use. The citation formats can be found in the links in the [references section](#references).

* Weighted Incomplete MaxSAT - Open-WBO-Inc-Cluster, Open-WBO-Inc-BMO, clustering of weights: [2]
* Unweighted Incomplete MaxSAT - Open-WBO-Inc-MCS, Open-WBO-Inc-OBV: [4]
* Other components from Open-WBO: [1]

## References
1. Ruben Martins, Vasco Manquinho, Inês Lynce. (2014) Open-WBO: A Modular MaxSAT Solver,. In: Sinz C., Egly U. (eds) Theory and Applications of Satisfiability Testing – SAT 2014. SAT 2014. Lecture Notes in Computer Science, vol 8561. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007/978-3-319-09284-3_33)]
2. Saurabh Joshi, Prateek Kumar, Ruben Martins, Sukrut Rao. (2018) Approximation Strategies for Incomplete MaxSAT. In: Hooker J. (eds) Principles and Practice of Constraint Programming. CP 2018. Lecture Notes in Computer Science, vol 11008. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007%2F978-3-319-98334-9_15)]
3. Saurabh Joshi, Prateek Kumar, Vasco Manquinho, Ruben Martins, Alexander Nadel and Sukrut Rao. Open-WBO-Inc in MaxSAT Evaluation 2018. MaxSAT Evaluation 2018, p.16. [[Link](https://helda.helsinki.fi/bitstream/handle/10138/237139/mse18_proceedings.pdf?sequence=1#page=17)]
4. Alexander Nadel. (2018) Solving MaxSAT with Bit-Vector Optimization. In: Beyersdorff O., Wintersteiger C. (eds) Theory and Applications of Satisfiability Testing – SAT 2018. SAT 2018. Lecture Notes in Computer Science, vol 10929. Springer, Cham [[Link](https://link.springer.com/chapter/10.1007/978-3-319-94144-8_4)]
