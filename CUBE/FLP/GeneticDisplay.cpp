#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BOLDGREEN   "\033[1m\033[32m"
#define RED     "\033[31m"

#include "GeneticDisplay.h"

#include <iostream>

using namespace std;
void GeneticDisplay::start(const GeneticProblem &gp) {
    cout << RED << "Search started" << RESET << endl;
    cout << "Generation " << gp.getCurrentGeneration();
    cout << " fitness: " << gp.getBestGene()->getFitness() << endl;
}

void GeneticDisplay::step(const GeneticProblem &gp) {
    cout << "Generation " << gp.getCurrentGeneration();
    cout << " best solution fitness: " << gp.getBestGene()->getFitness() << endl;
}

void GeneticDisplay::stop(const GeneticProblem &gp) {
    cout << GREEN<<"Search stopped" << endl;
    cout << " Best solution founded: "<< BOLDGREEN << *gp.getBestGene();
    cout << GREEN << " fitness: " << BOLDGREEN << gp.getBestGene()->getFitness() << RESET << endl;
}
