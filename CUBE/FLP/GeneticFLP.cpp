
#include "GeneticFLP.h"

#include <utility>
#include <limits>
#include <iostream>

using namespace std;
GeneticFLP::GeneticFLP(int populationSize, int numGeneration, int genesSize, double mutationRate,
                       std::vector<double> facilityCost, std::vector<std::vector<double>> transportCost)
        : GeneticProblem(populationSize, numGeneration, genesSize, mutationRate), facility_cost(std::move(facilityCost)),
          transport_cost(std::move(transportCost)) {}

double GeneticFLP::fitness(std::shared_ptr<Genes> gs) {
    double facility_sum = 0.;
    vector<int> facilities_indexes;
    vector<int> customers_indexes;
    for (int i = 0; i < gs->size(); ++i) {
        if ((*gs)[i]) {
            facility_sum += facility_cost[i];
            facilities_indexes.push_back(i);
        }
        else
            customers_indexes.push_back(i);
    }

    double transport_sum = 0.;
    for (int i : customers_indexes){
        double min_dist = 1e20; //cannot be infinity because of the probability of the solution 0000 in the roulette wheel
        for (int j : facilities_indexes){
            double d = transport_cost[i][j];
            if (d < min_dist)
                min_dist = d;
        }
        transport_sum += min_dist;
    }

    return facility_sum + transport_sum;
}

bool GeneticFLP::is_maximizing() const{
    return false;
}

GeneticConstrainedFLP::GeneticConstrainedFLP(int populationSize, int numGeneration, int genesSize, double mutationRate,
                                             const std::vector<double> &facilityCost,
                                             const std::vector<std::vector<double>> &transportCost, double maxDistance,
                                             double penalty) : GeneticFLP(populationSize,
                                                                          numGeneration,
                                                                          genesSize,
                                                                          mutationRate,
                                                                          facilityCost,
                                                                          transportCost),
                                                               maxDistance(maxDistance),
                                                               penalty(penalty) {}

double GeneticConstrainedFLP::fitness(std::shared_ptr<Genes> gs) {
    double facility_sum = 0.;
    vector<int> facilities_indexes;
    vector<int> customers_indexes;
    for (int i = 0; i < gs->size(); ++i) {
        if ((*gs)[i]) {
            facility_sum += facility_cost[i];
            facilities_indexes.push_back(i);
        }
        else
            customers_indexes.push_back(i);
    }

    double transport_sum = 0.;
    double penalty_sum = 0.;
    for (int i : customers_indexes){
        double min_dist = 1e20; //cannot be infinity because of the probability of the solution 0000 in the roulette wheel
        for (int j : facilities_indexes){
            double d = transport_cost[i][j];
            if (d < min_dist)
                min_dist = d;
        }
        if (min_dist > maxDistance)
            penalty_sum += penalty;
        transport_sum += min_dist;
    }
    return facility_sum + transport_sum + penalty_sum;
}
