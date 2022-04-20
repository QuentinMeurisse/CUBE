#include "VariableMovePlayer.h"
#include "../GraphGame.h"
#include "../Invariants/ColorNodeInvariant.h"
#include "../Strats/SwapColorStrat.h"

#include <utility>
#include <map>
#include <limits>
#include <vector>
#include <iostream>

using namespace std;
VariableMovePlayer::VariableMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex)
        : MultiStratPlayer(maxIndex), maxDistance(maxDistance), invariantKey(invariantKey), tokenIndex(tokenIndex) {}

VariableMovePlayer::VariableMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex,
                                       const std::function<bool(double, double)> &better)
        : MultiStratPlayer(better, maxIndex), maxDistance(maxDistance), invariantKey(invariantKey),
          tokenIndex(tokenIndex) {}

double VariableMovePlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    size_t numNodes = gg->numNodes();
    size_t cover_index = g->findNodeIndex(coverNodes[tokenIndex]);
    double sum = 0;
    for (int i = 0; i < numNodes; ++i) {
        double d = gg->distance(i, cover_index);
        if (d > maxDistance and g->getColor((*g)[i]) == "#ffffff")
            sum += d;
    }
    return sum;
}

std::vector<std::shared_ptr<Strategy>> VariableMovePlayer::getAllStrat(std::shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> to_return;

    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    uint64_t cover = coverNodes[tokenIndex];
    auto n_succs = g->nSuccessors(cover, strat_index);
    for (uint64_t succ : n_succs){
        if (g->getColor(succ) == "#ffffff"){
            shared_ptr<SwapColorStrat> scs = make_shared<SwapColorStrat>(cover, succ);
            to_return.push_back(scs);
        }
    }

    return to_return;
}

std::shared_ptr<Strategy> VariableMovePlayer::getRandomStrat(std::shared_ptr<Game> game) {
    return std::shared_ptr<Strategy>();
}

bool VariableMovePlayer::isMaximizing() {
    return false;
}

void VariableMovePlayer::print(std::ostream &os) const {
    os << "Player " << " with token " << tokenIndex << " and color " << invariantKey << ". StratIndex: " << strat_index;
}

VariableCooperativeMovePlayer::VariableCooperativeMovePlayer(int maxIndex,
                                                             double maxDistance,
                                                             char invariantKey,
                                                             unsigned int tokenIndex)
        : VariableMovePlayer(maxIndex, maxDistance, invariantKey, tokenIndex) {}

VariableCooperativeMovePlayer::VariableCooperativeMovePlayer(int maxIndex,
                                                             double maxDistance,
                                                             char invariantKey,
                                                             unsigned int tokenIndex,
                                                             const function<bool(double, double)> &better)
        : VariableMovePlayer(maxIndex, maxDistance, invariantKey, tokenIndex, better) {}

double VariableCooperativeMovePlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    size_t numNodes = gg->numNodes();
    double sum = 0;
    for (int i = 0; i < numNodes; ++i) {
        double mindist = numeric_limits<double_t >::infinity();
        for (uint64_t covered : coverNodes){
            size_t cover_index = g->findNodeIndex(covered);
            double  d = gg->distance(i, cover_index);
            if (d < mindist)
                mindist = d;
        }
        if (mindist > maxDistance and g->getColor((*g)[i]) == "#ffffff")
            sum += mindist;
    }
    return sum;
}

VariableKCenterPlayer::VariableKCenterPlayer(int maxIndex, char invariantKey, unsigned int tokenIndex):
        VariableMovePlayer(maxIndex, 0, invariantKey, tokenIndex){}

VariableKCenterPlayer::VariableKCenterPlayer(int maxIndex, char invariantKey, unsigned int tokenIndex,
                                             const function<bool(double, double)> &better):
        VariableMovePlayer(maxIndex, 0, invariantKey, tokenIndex, better){}

double VariableKCenterPlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    size_t numNodes = gg->numNodes();
    double maxdist = -1 * numeric_limits<double>::infinity();
    for (int i = 0; i < numNodes; ++i) {
        double mindist = numeric_limits<double>::infinity();
        for (uint64_t covered : coverNodes){
            size_t cover_index = g->findNodeIndex(covered);
            double  d = gg->distance(i, cover_index);
            if (d < mindist)
                mindist = d;
        }
        if (mindist > maxdist)
            maxdist = mindist;
    }
    return maxdist;
}

bool VariableKCenterPlayer::isMaximizing() {
    return false;
}
