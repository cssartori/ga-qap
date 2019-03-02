#ifndef __DATA_FILE_H__
#define __DATA_FILE_H__

#include <vector>
#include <string>
#include "solution.hpp"

class DataFile {
public:
	/*Flow and Distance matrix (usually matrix A and B in the MIP formulation)*/
	std::vector< std::vector<int> > flow;
	std::vector< std::vector<int> > distance;
	/*Number of facilities to install*/
	size_t n;

	DataFile();
	int read(const std::string& filename);
	int write_solution(const std::string& filename, const Solution& s);
	void print();


};

#endif //__DATA_FILE_H__
