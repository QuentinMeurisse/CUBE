#include "GeneticProblem.h"

#include <random>
#include <algorithm>
#include <iostream>

using namespace std;
GeneticProblem::GeneticProblem(int populationSize, int numGeneration, int genesSize, double mutationRate)
        : population_size(populationSize), num_generation(numGeneration),
          genes_size(genesSize), mutation_rate(mutationRate){
    if (mutationRate < 0 or mutationRate > 1)
        throw invalid_argument("The mutation rate must be in [0, 1].");
}

std::deque<std::shared_ptr<Genes>> GeneticProblem::generate_initial_population() const {
    deque<shared_ptr<Genes>> population;
    while (population.size() < population_size){
        vector<bool> genes;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 1);
        for (int i = 0; i < genes_size; ++i) {
            if (forced0->find(i) != forced0->end()){
                genes.push_back(false);
            } else if (forced1->find(i) != forced1->end()){
                genes.push_back(true);
            } else{
               genes.push_back(dist(gen));
            }
        }
        shared_ptr<Genes> g = make_shared<Genes>(genes);
        g->setForced0(forced0);
        g->setForced1(forced1);
        population.push_back(g);
    }
    return population;
}

void GeneticProblem::compute_fitness(const deque<shared_ptr<Genes>> &population) {
    for (auto const& gene : population){
        double fit = fitness(gene);
        gene->setFitness(fit);
    }
}

deque<shared_ptr<Genes>> GeneticProblem::generate_new_population(deque<shared_ptr<Genes>> &population) {
    deque<shared_ptr<Genes>> new_population;
    random_device rd;
    mt19937 gen(rd());
    shuffle(population.begin(),  population.end(), gen);
    for (int i = 0; i < population.size()-1; i += 2) {
        auto g1 = population[i];
        auto g2 = population[i+1];
        auto [ng1, ng2] = Genes::one_point_crossover(*g1, *g2);

        if (mutation_rate == 1){
            ng1.mutate();
            ng2.mutate();
        }
        else{
            uniform_real_distribution dist(0., 1.);
            if (dist(gen) < mutation_rate)
                ng1.mutate();
            if (dist(gen) < mutation_rate)
                ng2.mutate();
        }
        shared_ptr<Genes> ng1_ptr = make_shared<Genes>(ng1);
        shared_ptr<Genes> ng2_ptr = make_shared<Genes>(ng2);

        new_population.push_back(ng1_ptr);
        new_population.push_back(ng2_ptr);
    }
    compute_fitness(new_population);

    new_population.insert(new_population.end(), population.begin(),  population.end());
    return new_population;
}

deque<shared_ptr<Genes>> GeneticProblem::selection(deque<shared_ptr<Genes>> &population, int num_selected) const {
    deque<shared_ptr<Genes>> selected;
    for (int i = 0; i < num_selected; ++i) {
        double min_v = population.front()->getFitness(); // Population supposed to be sorted
        double max_v = population.back()->getFitness(); // Population supposed to be sorted
        if (min_v !=  max_v){
            vector<double> proba(population.size());
            auto accumulate_fitness = [&](double const& x, shared_ptr<Genes> const& g){
                return x + g->getFitness();
            };
            double fit_sum = accumulate(population.begin(),  population.end(), 0., accumulate_fitness);
            auto compute_proba = [&](shared_ptr<Genes> const& g){
                double p = g->getFitness() / fit_sum;
                return is_maximizing() ? p : (1. - p)/static_cast<double>(population.size() -1);
            };
            transform(population.begin(),  population.end(), proba.begin(), compute_proba);
            partial_sum(proba.begin(),  proba.end(), proba.begin());

            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution dist(0., 1.);
            double x = dist(gen);
            for (int j = 0; j < proba.size(); ++j) {
                if (x < proba[j]){
                    selected.push_back(population[j]);
                    population.erase(population.begin() +j);
                    break;
                }
            }

        }else{
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, int(population.size() - 1));
            int j = dist(gen);
            selected.push_back(population[j]);
            population.erase(population.begin() +j);
        }

    }
    return selected;
}

void GeneticProblem::genetic_search() {
    deque<shared_ptr<Genes>> current_population = generate_initial_population();
    compute_fitness(current_population);
    sort(current_population.begin(),  current_population.end());
    best_gene = is_maximizing() ? current_population.back() : current_population.front();
    current_generation = 1;
    for (auto const& l : listeners)
        l->start(*this);

    while (current_generation < num_generation){
        auto new_population = generate_new_population(current_population);
        sort(new_population.begin(),  new_population.end(), [&](shared_ptr<Genes> const& a, shared_ptr<Genes> const& b){return *a < *b;});
        auto nbest = is_maximizing() ? new_population.back() : new_population.front();
        if ((is_maximizing() and *nbest > *best_gene) or (not is_maximizing() and *nbest < *best_gene))
            best_gene = nbest;
        is_maximizing() ? new_population.pop_back() : new_population.pop_front();
        current_population = selection(new_population, population_size-1); //because the best gene is always selected
        current_population.push_front(nbest);
        current_generation++;
        for (auto const& l: listeners)
            l->step(*this);
    }
    for (auto const& l : listeners)
        l->stop(*this);
}

const shared_ptr<Genes> &GeneticProblem::getBestGene() const {
    return best_gene;
}

void GeneticProblem::add_listener(const shared_ptr<GeneticListener> &l) {
    listeners.push_back(l);
}

int GeneticProblem::getCurrentGeneration() const {
    return current_generation;
}

int GeneticProblem::getNumGeneration() const {
    return num_generation;
}

void GeneticProblem::setForced0(const shared_ptr<std::set<int>> &f0) {
    GeneticProblem::forced0 = f0;
}

void GeneticProblem::setForced1(const shared_ptr<std::set<int>> &f1) {
    GeneticProblem::forced1 = f1;
}




