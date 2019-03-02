#include "../include/solution.hpp"

Solution::Solution(int n){
	this->fitnessf = 0;
	this->permutationf.assign(n, -1);
}

Solution::Solution(std::vector<int> perm){
	this->fitnessf = 0;
	this->permutationf = perm;
}

std::vector<int> Solution::permutation(){
	return this->permutationf;	
}

int& Solution::operator[](int i){
	return this->permutationf[i];
}

int Solution::operator[](int i) const{
	return this->permutationf[i];
}

int Solution::fitness() const{
	return this->fitnessf;
}

void Solution::set_fitness(int new_fitness){
	this->fitnessf = new_fitness;
}

bool Solution::operator<(const Solution& s) const{
	return this->fitness() < s.fitness();
}

bool Solution::operator<=(const Solution& s) const{
	return this->fitness() <= s.fitness();
}

bool Solution::operator>(const Solution& s) const{
	return this->fitness() > s.fitness();
}

bool Solution::operator>=(const Solution& s) const{
	return this->fitness() >= s.fitness();
}
