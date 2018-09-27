# Installation Instructions for the Open-WBO-Inc MaxSAT Solver
The installation procedure for Open-WBO-Inc is identical to that of Open-WBO 2.0, and is described below.

Open-WBO-Inc is compiled like MiniSAT. The following are the different modes in which Open-WBO-Inc can be compiled:
* To compile Open-WBO-Inc statically ("s") in release mode ("r"), use
```
$ make rs
```
* To compile Open-WBO-Inc dynamically in debug mode, use
```
$ make
```
* To compile Open-WBO-Inc as a static library, use
```
$ make libr
```

To uninstall and clear all built files, use
```
$ make clean
```

Any MiniSAT-like SAT solver may be used with Open-WBO-Inc.
If you want to use a new SAT solver (e.g. "MiniSAT2.0"), it is necessary to 
crease a subdirectory (e.g. "minisat2.0") inside the [solvers directory](solvers/) and 
change the [Makefile](Makefile) accordingly:

```
# VERSION    = core or simp
# SOLVERNAME = name of the SAT solver
# SOLVERDIR  = subdirectory of the SAT solver
# NSPACE     = namespace of the SAT solver
#
# e.g. minisat compilation with core version:
#
# VERSION    = core
# SOLVERNAME = "Minisat"
# SOLVERDIR  = minisat
# NSPACE     = Minisat
#
VERSION    = core
SOLVERNAME = "MiniSAT2.0"
SOLVERDIR  = minisat2.0
NSPACE     = MiniSAT
# THE REMAINING OF THE MAKEFILE SHOULD BE LEFT UNCHANGED
```

The following SAT solvers are included in this version of Open-WBO-Inc: 
* [minisat2.2](solvers/minisat2.2) 
* [glucose4.0](solvers/glucose4.0)
* [glucose4.1](solvers/glucose4.1)
* [MapleSAT](solvers/MapleCOMSPS_LRB)


