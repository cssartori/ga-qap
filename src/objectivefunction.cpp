#include "../include/objectivefunction.hpp"
#include <iostream>

ObjectiveFunction::ObjectiveFunction(const DataFile& _data):data(_data){ }

int ObjectiveFunction::eval(const Solution& s) const{
	int fitness = 0;
	/*Function to minimize*/
	for(size_t i=0;i<data.n;i++){
		for(size_t j=0;j<data.n;j++){
			/*There is a flow between two locations i and j, but also there is a distance between facilities p(i) and p(j)*/
			fitness += data.flow[i][j]*data.distance[s[i]][s[j]];
		}
	}

	return fitness;
}

/*Evaluates a pair exchange in solution s, where locations in s(i) and s(j) will be exchanged*/
int ObjectiveFunction::eval_pair_change(const Solution& s, size_t i, size_t j) const{
	int fitness = s.fitness();
	int delta = 0;	
	
	delta +=  data.flow[i][j]*data.distance[s[i]][s[j]];
	delta = delta - data.flow[i][j]*data.distance[s[j]][s[i]];
	delta +=  data.flow[j][i]*data.distance[s[j]][s[i]];
	delta = delta - data.flow[j][i]*data.distance[s[i]][s[j]];

	delta = delta + (2*data.flow[i][i]*data.distance[s[i]][s[i]]);
	delta = delta - (2*data.flow[i][i]*data.distance[s[j]][s[j]]);
	delta = delta + (2*data.flow[j][j]*data.distance[s[j]][s[j]]);
	delta = delta - (2*data.flow[j][j]*data.distance[s[i]][s[i]]);

	for(size_t k=0;k<data.n;k++){
		if(k == i || k==j)
			continue;

		delta = delta + data.flow[k][i]*data.distance[s[k]][s[i]];
		delta = delta - data.flow[k][i]*data.distance[s[k]][s[j]];
		delta = delta + data.flow[i][k]*data.distance[s[i]][s[k]];
		delta = delta - data.flow[i][k]*data.distance[s[j]][s[k]];

		delta = delta + data.flow[k][j]*data.distance[s[k]][s[j]];
		delta = delta - data.flow[k][j]*data.distance[s[k]][s[i]];
		delta = delta + data.flow[j][k]*data.distance[s[j]][s[k]];
		delta = delta - data.flow[j][k]*data.distance[s[i]][s[k]];
	}

	fitness -= delta;
	return fitness;
}
