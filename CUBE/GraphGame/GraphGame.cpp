#include "GraphGame.h"
#include "../DataStructures/GraphUtility.h"

#include <utility>
#include <memory>
#include <thread>
using namespace std;

GraphGame::GraphGame(shared_ptr<CoordGraph> g): g(move(g)) {
    unsigned int cores = thread::hardware_concurrency();
    sp = this->g->multicoreDijkstra(ProblemType::shortest_path, cores - 2);
}

GraphGame::GraphGame(shared_ptr<CoordGraph> g, bool load, string const &path) : g(move(g)) {
    if (not load) {
        unsigned int cores = thread::hardware_concurrency();
        sp = this->g->multicoreDijkstra(ProblemType::shortest_path, cores - 2);
        GraphUtility::writeMatrix(sp, path);
    }
    else
        sp = GraphUtility::loadMatrix<double>(path);
}

double GraphGame::distance(unsigned int n1, unsigned int n2) {
    return sp[n1][n2];
}

void GraphGame::setNodeColor(uint64_t n, const std::string& color) {
    g->setColor(n, color);
}

string GraphGame::getNodeColor(uint64_t n) {
    return g->getColor(n);
}

void GraphGame::updateAll(const std::shared_ptr<Strategy> &s) {
    for(const auto& inv : invariants){
        inv.second->update(s);
    }
}

void GraphGame::undoAll(const std::shared_ptr<Strategy> &s) {
    for(const auto& inv : invariants){
        inv.second->undo(s);
    }
}

void GraphGame::addInvariant(std::shared_ptr<Invariant> inv, char const &key) {
    invariants[key] = move(inv);
}

const map<char, std::shared_ptr<Invariant>> &GraphGame::getInvariants() const {
    return invariants;
}

size_t GraphGame::numNodes() {
    return g->size();
}

const shared_ptr<CoordGraph> & GraphGame::getG() const {
    return g;
}

void GraphGame::removeInvariant(const char &key) {
    invariants.erase(key);
}


