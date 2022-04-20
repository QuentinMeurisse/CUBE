#ifndef COMODGAMECPP_GENETICEASY_H
#define COMODGAMECPP_GENETICEASY_H

#include "GeneticProblem.h"

class GeneticEasy : public GeneticProblem{

public:
    GeneticEasy(int populationSize, int numGeneration, int genesSize, double mutationRate);

    double fitness(std::shared_ptr<Genes> gs) override;

    [[nodiscard]] bool is_maximizing() const override;

};


#endif //COMODGAMECPP_GENETICEASY_H
