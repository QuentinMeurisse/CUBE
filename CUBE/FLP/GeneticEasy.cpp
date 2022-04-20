
#include "GeneticEasy.h"

GeneticEasy::GeneticEasy(int populationSize, int numGeneration, int genesSize, double mutationRate) : GeneticProblem(
        populationSize, numGeneration, genesSize, mutationRate) {}

double GeneticEasy::fitness(std::shared_ptr<Genes> gs) {
    int sum = 0;
    for (int i = 0; i < gs->size(); ++i) {
        if ((*gs)[i])
            sum++;
    }
    return sum;
}

bool GeneticEasy::is_maximizing() const{
    return true;
}
