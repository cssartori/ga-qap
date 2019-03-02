#include "../include/datafile.hpp"
#include "../include/solution.hpp"
#include "../include/populationcreator.hpp"
#include "../include/objectivefunction.hpp"
#include "../include/localsearch.hpp"
#include "../include/geneticalgorithm.hpp"
#include "../include/timer.hpp"
#include <iostream>
#include <cstdio>

using namespace std;

char *input_file = NULL;
char *output_file = NULL;
size_t pop_size = 0;
size_t max_iterations = 3000;
size_t tournament_size = 4;
double cross_p = 0.7;
int seed = 1;
bool prints = false;

/*Reads the parameter for the problem*/
void read_parameter(int argc, char **argv);

int main(int argc, char **argv){
	/*Read input parameters*/
	read_parameter(argc, argv);

	DataFile data;
	if(data.read(input_file) == -1){
		printf("Error opening file \'%s\'.\n", input_file);
		return -1;
	}

	if(pop_size == 0) pop_size = data.n*0.1;

	if(seed == -1) seed = time(NULL);

	Timer t;
	t.start();

	//create and run GA
	GeneticAlgorithm ga(data, pop_size, max_iterations, tournament_size, cross_p, seed, prints);
	Solution best = ga.run();

	t.stop();
	
	if(prints){
		printf("Initial Solution = %i\n", ga.get_initial_solution().fitness());
		printf("Best Solution Found = %i \n", best.fitness());
		printf("Iterations = %li \n", ga.get_iter_best());
		printf("Time = %.2f \n", t.elapsed_seconds());
	}
	
	if(output_file != NULL){
		if(data.write_solution(output_file, best) < 0){
			printf("Failed to save solution to \'%s\'.\n", output_file);
			return -1;
		}
		if(prints)
			printf("Solution saved to \'%s\'.\n", output_file);
	}
	
	if(prints)
		printf("Success!\n");

	return 0;
}



/*Reads the parameter for the problem*/
void read_parameter(int argc, char **argv){

	if(argc < 3){
		cout << "Usage:\n" << argv[0] << " -f [input file path]" << endl;
		exit(-1);
	}

	int i=1;
	if(argv[i][0] != '-'){
		cout << "Parameter " << argv[i] << "not defined." << endl;
		exit(-1);
	}else if(argv[i][1] != 'f'){
		cout << "Expected first parameter -f [input file path], found " << argv[i] << "." << endl;
		exit(-1);
	}

	i++;
	input_file = argv[i]; //input file path

	for(i=3;i<argc;i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'p'://population size
					i++;
					pop_size = atoi(argv[i]);
					break;
				case 's'://random seed
					i++;
					seed = atoi(argv[i]);
					break;
				case 'i'://max iterations
					i++;
					max_iterations = atoi(argv[i]);
					break;
				case 'c'://crossover probability
					i++;
					cross_p = atof(argv[i]);
					break;
				case 'k'://tournament size
					i++;
					tournament_size = atoi(argv[i]);
					break;
				case 'o'://output file path
					i++;
					output_file = argv[i];
					break;
				case 'm'://print out allowed
					prints = true;
					break;
				default:
					cout << "Parameter " << argv[i] << "not defined." << endl;
					exit(-1);
			}
		}else{
			cout << "Parameter " << argv[i] << "not defined." << endl;
			exit(-1);
		}
	}
}




