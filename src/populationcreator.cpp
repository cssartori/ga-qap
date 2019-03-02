#include "../include/populationcreator.hpp"

/*Random population creator*/
std::vector<Solution> PopulationCreator::random(const size_t max_pop_size, const size_t n, const ObjectiveFunction& of, std::default_random_engine& generator){
	std::vector<Solution> population;
	std::uniform_int_distribution<int> randn(0, n-1);
	
	/*While population is smaller than the parameter 'population size'*/
	while(population.size() < max_pop_size){
		std::vector<int> individual;
		std::vector<bool> used(n, false);
		
		/*While individual doesn't have n locations in it*/
		while(individual.size() < n){
			int r = randn(generator);
			if(not used[r]){
				individual.push_back(r);
				used[r] = true;
			}
		}

		Solution si(individual);
		si.set_fitness(of.eval(si));
		population.push_back(si);
	}
	
	return population;
}	
