#ifndef CUBE_GENETICPROBLEM_H
#define CUBE_GENETICPROBLEM_H


#include "Genes.h"

#include <memory>
#include <vector>
#include <deque>
#include <set>

class GeneticProblem;

class GeneticListener{
public:
    virtual void start(GeneticProblem const& gp) = 0;

    virtual void step(GeneticProblem const& gp) = 0;

    virtual void stop(GeneticProblem const& gp) = 0;
};

class GeneticProblem {
public:

    GeneticProblem(int populationSize, int numGeneration, int genesSize, double mutationRate);


    void genetic_search();

    [[nodiscard]] const std::shared_ptr<Genes> &getBestGene() const;

    [[nodiscard]] int getCurrentGeneration() const;

    [[nodiscard]] int getNumGeneration() const;

    void add_listener(std::shared_ptr<GeneticListener> const& l);

    void setForced0(const std::shared_ptr<std::set<int>> &f0);

    void setForced1(const std::shared_ptr<std::set<int>> &f1);

    virtual double fitness(std::shared_ptr<Genes> gs) = 0;

    [[nodiscard]] virtual bool is_maximizing() const = 0;

protected:
    int population_size;
    int num_generation;
    int genes_size;
    double mutation_rate;
    int current_generation = 1; //Allow to the listeners to display the current generation
    std::shared_ptr<Genes> best_gene;
    std::vector<std::shared_ptr<GeneticListener>> listeners{};
    std::shared_ptr<std::set<int>> forced0 = std::make_shared<std::set<int>>();
    std::shared_ptr<std::set<int>> forced1 = std::make_shared<std::set<int>>();

    [[nodiscard]] std::deque<std::shared_ptr<Genes>> generate_initial_population() const;

    /**
     *
     * @param population
     * @return compute the fitness of each solution in population
     */
    void compute_fitness(const std::deque<std::shared_ptr<Genes>> &population);

    /**
    *
    * @param population
    * @return a new population doing one-point crossovers from population and doing mutation with probability
     * mutation_rate
    */
    std::deque<std::shared_ptr<Genes>>
    generate_new_population(std::deque<std::shared_ptr<Genes>> &population);

    /**
     *
     * @param population a list of n genes supposed to be sorted by their fitness
     * @return n/2 genes selected by the roulette wheel method
     */
    [[nodiscard]] std::deque<std::shared_ptr<Genes>>
    selection(std::deque<std::shared_ptr < Genes>> &population, int num_selected) const;



};


#endif //CUBE_GENETICPROBLEM_H
