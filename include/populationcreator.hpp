#ifndef __POPULATION_CREATOR_H__
#define __POPULATION_CREATOR_H__

#include "solution.hpp"
#include "objectivefunction.hpp"
#include <random>
#include <vector>

class PopulationCreator{
public:
	/*Random population creator*/
	static std::vector<Solution> random(const size_t max_pop_size, const size_t n, const ObjectiveFunction& of, std::default_random_engine& generator);	

};

#endif //__POPULATION_CREATOR_H__
