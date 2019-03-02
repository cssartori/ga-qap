#ifndef __OBJECTIVE_FUNCTION_H__
#define __OBJECTIVE_FUNCTION_H__

#include "datafile.hpp"
#include "solution.hpp"

class ObjectiveFunction{
private:
	/*Data information about the problem*/
	const DataFile& data;
	
public:
	ObjectiveFunction(const DataFile& _data);
	/*Evaluates entire solution s*/
	int eval(const Solution& s) const;
	/*Evaluates a pair exchange in solution s, where locations in s(i) and s(j) will be exchanged*/
	int eval_pair_change(const Solution& s, size_t i, size_t j) const;
	
};

#endif //__OBJECTIVE_FUNCTION_H__

