#include "../include/localsearch.hpp"
#include <cstdio>
#include <utility>
#include <set>

LocalSearch::LocalSearch(const DataFile& _data, const ObjectiveFunction& _of):data(_data), of(_of){ }

/*Finds a local minima for this solution changing all pairs (t,u) of places in permutation s*/
Solution LocalSearch::all_pair_changes(Solution s){

	for(size_t t=0;t<data.n;t++){
		for(size_t u=t+1;u<data.n;u++){
			int f = of.eval_pair_change(s, t, u);

			if(f < s.fitness()){
				int temp = s[t];
				s[t] = s[u];
				s[u] = temp;
				s.set_fitness(f);
			}
		}
	}
	return s;
}

/*Run a limited iterated local search in solution s*/
Solution LocalSearch::limited_iterated_search(Solution s, int max_iterations, std::default_random_engine& generator){	
	Solution best = s;
	/*Start picking a neighbor of s*/
	s = all_pair_changes(s);
	if(s < best) best = s;	

	size_t u = 2;
	int iter = 0;
	while(iter < max_iterations*0.1){
		iter++;
		/*Perturb the current solution s with u pair changes*/
		perturbation(s, u, generator);
		
		/*Updates u variable*/
		u++;
		if(u > data.n) u = 2;
		
		/*Improves s locally*/
		s = all_pair_changes(s);

		if(s < best) best = s;
        else s = best;
	}	
	
	return best;
}

/*Makes u pair perturbations in solution s*/
void LocalSearch::perturbation(Solution& s, int u, std::default_random_engine& generator){
	std::uniform_int_distribution<int> rand_place(0,data.n-1);
	int p = 0;
	
	while(p < u){
		p++;
		/*Pick two locations randomly*/
		int t = rand_place(generator);
		int v = rand_place(generator);	
		while(v == t)
			v = rand_place(generator);	

		/*Evaluate the exchange*/
		int f = of.eval_pair_change(s, t, v);
		/*Exhcange points (wheter it is better or not)*/
		s.set_fitness(f);
		int temp = s[t];
		s[t] = s[v];
		s[v] = temp;
	}
}

