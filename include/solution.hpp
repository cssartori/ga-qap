#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <vector>

/*Chromossome class of a possible solution to the problem*/
class Solution{
private:
	/*A possible permutation of the facilities between locations*/
	std::vector<int> permutationf;
	/*Fitness of this permutation*/
	int fitnessf;

public:
	Solution(int n=0);
	Solution(std::vector<int> perm);
	std::vector<int> permutation();
	//int& operator[](int i);
	int& operator[](int i);
	int operator[](int i) const;
	int fitness() const;
	void set_fitness(int new_fitness);
	bool operator<(const Solution& s) const;
	bool operator<=(const Solution& s) const;
	bool operator>(const Solution& s) const;
	bool operator>=(const Solution& s) const;

};

#endif //__SOLUTION_H__
