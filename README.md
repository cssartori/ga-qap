# Hybrid Genetic Algorithm Applied to the Quadratic Assignment Problem #

This programs applies an Hybrid Genetic Algorithm to solve the *Quadratic Assignment Problem* (QAP). It is based on the work of [Mise 04], where a HGA is proposed with a *Limited Tabu Search* for local improvement of solutions.

## The Method ##

The GA works with the following configuration:

* Tournament selection of parents;
* Optmized ULX *crossover*; 
* Iterated Local Search with All Pair Swap search;
* *Shift Mutation* and perturbation in case of convergence;
* The population size is 0.1*N by default (considering N the QAP size);
* Elitism in the population, keeping 50% of the best solutions at every generation.

## Implementation ##

The code was implemented in C++11 using the STL library. It can be compiled under UNIX systems using a *makefile*.

## Benchmarks ##

Instances and their best-known solutions can be found under the folder *instances/*. All of them are from the QAPLIB, which can be found [here](http://anjos.mgi.polymtl.ca/qaplib/inst.html).

## About ##

Project done for the "INF01037-Evolutionary Computing" class (2016) at UFRGS.

