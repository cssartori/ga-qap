#ifndef __LOCAL_SEARCH_H__
#define __LOCAL_SEARCH_H__

#include "datafile.hpp"
#include "solution.hpp"
#include "objectivefunction.hpp"
#include <random>
#include <vector>

class LocalSearch{
private:
	/*Basic data needed*/
	const DataFile& data;
	/*Evaluator*/
	const ObjectiveFunction& of;

	/*Makes u pair perturbations in solution s*/
	void perturbation(Solution& s, int u, std::default_random_engine& generator);

public:
	LocalSearch(const DataFile& _data, const ObjectiveFunction& _of);

	/*Run a limited iterated local search in solution s*/
	Solution limited_iterated_search(Solution s, int max_iterations, std::default_random_engine& generator);

	/*Finds a local minima for this solution changing all pairs (t,u) of places in permutation s*/
	Solution all_pair_changes(Solution s);


};

#endif //__LOCAL_SEARCH_H__
