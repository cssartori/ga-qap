#include "../include/datafile.hpp"
#include <cstdio>
#include <iostream>

DataFile::DataFile(){ }

int DataFile::read(const std::string& filename){
	FILE *file = fopen(filename.c_str(), "rt");

	if(file == NULL)
		return -1;
	int temp;
	//reads the number of facilities
	temp = fscanf(file, "%li\n\n", &(this->n));
	
	this->flow.assign(this->n, std::vector<int>(this->n, 0));
	this->distance.assign(this->n, std::vector<int>(this->n, 0));

	//reads flow and distance matrix
	for(size_t i=0;i<this->n;i++)
		for(size_t j=0;j<this->n;j++)
			temp = fscanf(file, "%i", &(this->flow[i][j]));

	for(size_t i=0;i<this->n;i++)
		for(size_t j=0;j<this->n;j++)
			temp = fscanf(file, "%i", &(this->distance[i][j]));

	/*Just to avoid compilation warnings*/
	temp+=1;

	fclose(file);
	return 0;
}

int DataFile::write_solution(const std::string& filename, const Solution& s){
	FILE *file = fopen(filename.c_str(), "wt");
	
	if(file == NULL)
		return -1;

	fprintf(file, "%li %i\n", n, s.fitness());
	for(size_t i=0;i<n;i++){
		fprintf(file, "%i ", s[i]);
	}
	fprintf(file, "\n");

	fclose(file);
	return 0;
}

void DataFile::print(){
	std::cout << "====================================" << std::endl;
	std::cout << "n = " << this->n << std::endl;
	
	std::cout << "A :" << std::endl;
	for(size_t i=0;i<this->n;i++){
		for(size_t j=0;j<this->n;j++){
			std::cout << this->flow[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "B :" << std::endl;
	for(size_t i=0;i<this->n;i++){
		for(size_t j=0;j<this->n;j++){
			std::cout << this->distance[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
