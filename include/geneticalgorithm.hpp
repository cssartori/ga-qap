#ifndef __GENETIC_ALGORITHM_H__
#define __GENETIC_ALGORITHM_H__

#include "datafile.hpp"
#include "solution.hpp"
#include "../include/objectivefunction.hpp"
#include <random>
#include <vector>

class GeneticAlgorithm{
private:
	/*The basic data of the problem*/
	const DataFile& data;
	/*Crossover probability*/
	double pc;
	/*Population size*/
	size_t pop_size;
	/*The tournament size*/
	size_t k;
	/*Maximum number of generations to run*/
	size_t max_generations;
	/*Random operator seed*/
	size_t seed;
	/*Indicates wheter prints are allowed or not*/
	bool prints;
	
	Solution best_solution;
	Solution initial_solution;
	size_t iter_best;

	/*Variables of the random number generator*/
	std::default_random_engine generator;
	
	/*Tournament selection of size k, except >= 0 is a solution that can't be selected*/
	Solution tournament(const std::vector<Solution>& population, size_t k, int& except);
	/*Shift the current solution u positions in a circular way*/
	void shift_mutation(Solution& s, size_t u);
	/*Crossover of two parents producing one child. Equal positions are kept, while the rest is randomly chosen (the best solution in N*0.2 runs is returned). There is a pc change that crossober occurs, otherwise the best parent is returned*/
	Solution crossover_opt(const Solution& p1, const Solution& p2, const ObjectiveFunction& of);


public:
	/*Constructor*/
	GeneticAlgorithm(const DataFile& _data, size_t pop_size, size_t max_generations, size_t k, double pc, size_t seed, bool prints=false);
	Solution run();
	Solution get_initial_solution();
	Solution get_best_solution();
	size_t get_iter_best();
	

};

#endif
