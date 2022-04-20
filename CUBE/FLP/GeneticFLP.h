#ifndef CUBE_GENETICFLP_H
#define CUBE_GENETICFLP_H

#include "GeneticProblem.h"

#include <vector>

class GeneticFLP : public GeneticProblem{
public:
    GeneticFLP(int populationSize,
               int numGeneration,
               int genesSize,
               double mutationRate,
               std::vector<double> facilityCost,
               std::vector<std::vector<double>> transportCost);

    double fitness(std::shared_ptr<Genes> gs) override;

    [[nodiscard]] bool is_maximizing() const override;

protected:
    std::vector<double> facility_cost;
    std::vector<std::vector<double>> transport_cost;
};

class GeneticConstrainedFLP : public GeneticFLP{
public:
    GeneticConstrainedFLP(int populationSize,
                          int numGeneration,
                          int genesSize,
                          double mutationRate,
                          const std::vector<double> &facilityCost,
                          const std::vector<std::vector<double>> &transportCost,
                          double maxDistance,
                          double penalty);

    double fitness(std::shared_ptr<Genes> gs) override;

private:
    double maxDistance;
    double penalty;
};


#endif //CUBE_GENETICFLP_H
