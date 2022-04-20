#include "GeneticGUIListener.h"
#include "Genes.h"
#include "FLPColors.h"

#include <utility>
#include <iostream>
#include <chrono>
#include <thread>


using namespace std;
using namespace this_thread;
using namespace chrono_literals;

GeneticGUIListener::GeneticGUIListener(std::shared_ptr<CoordGraph> graph, std::vector<uint64_t> whites, bool slow_mode)
        : g(move(graph)), white_nodes(move(whites)), slow(slow_mode) {}

void GeneticGUIListener::start(const GeneticProblem &gp) {

}

void GeneticGUIListener::step(const GeneticProblem &gp) {
    const auto& best_gene = gp.getBestGene();
    for (int i = 0; i < white_nodes.size(); ++i) {
        if ((*best_gene)[i] and g->getColor(white_nodes[i]) != forcedFacilityColor)
            g->setColor(white_nodes[i], facilityColor);
        if (not (*best_gene)[i] and g->getColor(white_nodes[i]) != forcedCustomerColor)
            g->setColor(white_nodes[i], customerColor);
    }
    double x = (100. * gp.getCurrentGeneration()) / gp.getNumGeneration();
    int percent = int(x);
    emit generationChanged(percent);
    emit fitnessChanged(gp.getBestGene()->getFitness());
    emit factoriesChanged(gp.getBestGene()->num0(), gp.getBestGene()->num1());
    emit doUpdate();
    if (slow)
        sleep_for(200ms);
}

void GeneticGUIListener::stop(const GeneticProblem &gp) {

}
