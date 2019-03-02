#include "../include/geneticalgorithm.hpp"
#include "../include/populationcreator.hpp"
#include "../include/localsearch.hpp"
#include <algorithm>
#include <cstdio>

/*Constructor*/
GeneticAlgorithm::GeneticAlgorithm(const DataFile& _data, size_t pop_size, size_t max_generations, size_t k, double pc, size_t seed, bool prints) : data(_data){
	this->pop_size = pop_size;
	this->max_generations = max_generations;
	this->k = k;
	this->pc = pc;
	this->seed = seed;
	this->prints = prints;

	this->generator = std::default_random_engine(seed);
}


bool comp(const Solution& s1, const Solution& s2){
	return s1 < s2;
}

Solution GeneticAlgorithm::run(){
	/*Creates an objective function evaluator and a population creator*/
	ObjectiveFunction of(data);

	/*Creates random population as initial set*/
	std::vector<Solution> population = PopulationCreator::random(pop_size, data.n, of, generator);

	/*Local search object*/
	LocalSearch ls(data, of);

	/*Improves locally all the initial solutions*/
	for(size_t i=0;i<population.size();i++){
		population[i] = ls.all_pair_changes(population[i]);
	}
	/*Sorts population in increasing order*/
	sort(population.begin(), population.end());//, comp);
	/*Selects the best solution*/
	Solution best_solution = population[0];
	
	if(prints) printf("Best solution in generation 0:  %i\n", best_solution.fitness());
	
	/*Updates basic information attributes*/
	this->best_solution = best_solution;
	this->initial_solution = best_solution;
	this->iter_best = 0;
	
	/*Initialize basic variables of the GA*/
	size_t u = 1;
	size_t last_mutation = 0;
	int mutation_type = 1;
    size_t generation = 0;
	
	/*GA main loop*/
	while(generation < max_generations){
		generation++;
		std::vector<Solution> offspring;
		while(offspring.size() < pop_size*0.5){
			int except = -1;
			/*Tournament to select parents*/
			Solution p1 = tournament(population, k, except);
			Solution p2 = tournament(population, k, except);

			/*crossover of parents*/
			Solution c = crossover_opt(p1, p2, of);

			/*Evaluate c*/
			//c.set_fitness(of.eval(c));
			
			/*Run a limited local search on the new child*/
			c = ls.limited_iterated_search(c, data.n, generator);	

			offspring.push_back(c);
		}

		/*Sorts offspring in increasing order*/
		sort(offspring.begin(), offspring.end());//, comp);
		/*Inserts the best individuals from offspring in the population as long as they are better than the latter*/
		for(size_t i=0;i<offspring.size();i++){
			if(offspring[i] < population[pop_size-1-i])
				population[pop_size-1-i] = offspring[i];
		}
		
		/*Sorts population in increasing order*/
		sort(population.begin(), population.end());//, comp);
		
		/*In case population has converged*/
		if(population[0] >= best_solution && generation - last_mutation > 100 && generation - iter_best > 200 && mutation_type==1){ 
			/*First try the shift mutation approach*/
			for(size_t i=0;i<pop_size;i++){
				shift_mutation(population[i], u);
				population[i].set_fitness(of.eval(population[i]));
			}
			if(prints) printf("Mutated population with u = %li\n", u);

			u++;
			if(u > data.n){
				/*In case the shift mutation has not done the job, go to the second type*/
				u=1;
				mutation_type=2;
			}

			sort(population.begin(), population.end());//, comp);
			last_mutation = generation;
		}else if(population[0] >= best_solution && generation - last_mutation > 100 && generation - iter_best > 200 && mutation_type==2){
			/*The second approach executes several perturbations in the solutions*/
			if(prints) printf("Perturbing solutions...\n");

			std::uniform_int_distribution<int> rand_place (0,data.n-1);
			
			for(size_t i=0;i<pop_size;i++){
				for(size_t j=0;j<data.n*0.2;j++){
					int r = rand_place(generator);
					int s = rand_place(generator);
					while(r == s) r = rand_place(generator);

					population[i].set_fitness(of.eval_pair_change(population[i], r, s));
				
					int temp = population[i][r];
					population[i][r] = population[i][s];
					population[i][s] = temp;
				}
				
				/*Improves the solutions locally*/
				population[i] = ls.all_pair_changes(population[i]);
			}
			
			sort(population.begin(), population.end());//, comp);
			last_mutation = generation;
			mutation_type = 1;
		}
		
		/*If the new best solution is better than the current one*/
		if(population[0] < best_solution){
			/*Update the current best solution*/
			best_solution = population[0];
			this->best_solution = best_solution;
			this->iter_best = generation;
			u = 1;
			if(prints) printf("Best solution in generation %li:  %i\n", generation, best_solution.fitness());
		}
	}

	/*Finally, return the best solution found*/
	return best_solution;
}

/*Tournament selection of size k, except >= 0 is a solution that can't be selected*/
Solution GeneticAlgorithm::tournament(const std::vector<Solution>& population, size_t k, int& except){
	/*Selects a random individual from the population that is not the except one*/
	std::uniform_int_distribution<int> rand_pop(0, pop_size-1);
	int r = rand_pop(generator);

	while(r == except) r = rand_pop(generator);
	
	Solution selected = population[r];
	except = r;
	
	/*Runs the tournament k-1 times and selects the best from this tournament*/
	for(size_t i=1;i<k;i++){
		r = rand_pop(generator);
		while(r == except) r = rand_pop(generator);
		if(population[r] < selected){
			selected = population[r];
			except = r;
		}
	}

	/*Returns the best permutation*/
	return selected;
}

/*Crossover of two parents producing one child. Equal positions are kept, while the rest is randomly chosen (the best solution in N*0.2 runs is returned). There is a pc change that crossober occurs, otherwise the best parent is returned*/
Solution GeneticAlgorithm::crossover_opt(const Solution& p1, const Solution& p2, const ObjectiveFunction& of){
	/*probability of crossover occurring*/
	std::uniform_real_distribution<double> prob(0,1);
	double pcs = prob(generator);
	if(pcs > pc){ /*if crossover does not occur, return the best parent*/
		if(p1 > p2) return p2;
		else return p1;
	}

	/*The new child*/
	std::vector<int> child(data.n, -1);
	/*Vector indicating wheter a given facility was already placed or not*/
	std::vector<bool> placed(data.n, false);	

	/*Copy equal positions from parents into child*/
	for(size_t i=0;i<data.n;i++){
		if(p1[i] == p2[i]){
			child[i] = p1[i];
			placed[p1[i]] = true;
		}
	}
	
	/*Save the equal positions stored*/
	std::vector<int> childm = child;
	std::vector<bool> placedm = placed;
	Solution *best = NULL;
	/*Tries N*0.2 times*/
    int lim = data.n*0.2;
	std::uniform_int_distribution<int> rand_place (0,data.n-1);
	
	for(int m=0;m<lim;m++){
		child = childm;
		placed = placedm;

		/*Assign the rest of the positions according to parents or randomly*/
		for(size_t i=0;i<data.n;i++){
			if(child[i] == -1){
				if(not placed[p1[i]] && not placed[p2[i]]){
					/*If none of the parents position was already placed, choose probabilistically*/
					double p = prob(generator);
					if( p < 0.5  ) child[i] = p1[i];
					else child[i] = p2[i];
				}else if(not placed[p1[i]]){
					child[i] = p1[i];
				}else if(not placed[p2[i]]){
					child[i] = p2[i];
				}else{
					/*If both positions were placed, choose another randomly*/
					int r = rand_place(generator);
					while(placed[r]) r = rand_place(generator);
					child[i] = r;
				}
			
				/*Mark this place as used*/
				placed[child[i]] = true;
			}
		}
		Solution s(child);
		s.set_fitness(of.eval(s));

		if(best == NULL){
			best = new Solution();
			*best = s;
		}else if(*best > s){
			*best = s;
		}
		
	}

	return *best;
}	


/*Shift the current solution u positions in a circular way*/
void GeneticAlgorithm::shift_mutation(Solution& s, size_t u){
	std::vector<int> p1(u);
	std::vector<int> p2(data.n-u);

	for(size_t i=0;i<data.n;i++){
		if(i<u) p1[i] = s[i];
		else p2[i-u] = s[i];
	}

	for(size_t i=0;i<data.n;i++){
		if(i < (data.n-u)) s[i] = p2[i];
		else s[i] = p1[i-(data.n-u)];
	}
}

Solution GeneticAlgorithm::get_initial_solution(){
	return this->initial_solution;
}

Solution GeneticAlgorithm::get_best_solution(){
	return this->best_solution;
}

size_t GeneticAlgorithm::get_iter_best(){
	return this->iter_best;
}
