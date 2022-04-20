#include "SwapPlayer.h"
#include "../GraphGame.h"
#include "../Invariants/ColorNodeInvariant.h"
#include "../Strats/SwapColorStrat.h"

#include <utility>
#include <memory>
#include <limits>

using namespace std;

SwapPlayer::SwapPlayer(double maxDistance, std::string color, char invariantKey)
        : Player(), maxDistance(maxDistance), color(std::move(color)), invariantKey(invariantKey) {}

SwapPlayer::SwapPlayer(double maxDistance, std::string color, char invariantKey,
                       const std::function<bool(double, double)> &better)
        : Player(better), maxDistance(maxDistance), color(move(color)), invariantKey(invariantKey) {}

double SwapPlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    unsigned int numNodes = gg->numNodes();
    double toReturn = numeric_limits<double>::min();
    for (int i = 0; i < numNodes; ++i) {
        double mindist = numeric_limits<double>::max();
        for (uint covered : coverNodes){
            size_t covered_index = g->findNodeIndex(covered);
            if (gg->distance(i, covered_index) < mindist)
                mindist = gg->distance(i, covered_index);
        }
        if (mindist >maxDistance)
            toReturn += mindist;
    }
    return toReturn;
}

vector<std::shared_ptr<Strategy>> SwapPlayer::getAllStrat(std::shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;

    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> bni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = bni->getTokens();
    unsigned int numNodes = gg->numNodes();
    for (uint cover : coverNodes){
        for (uint i = 0; i < numNodes; i++){
            if (gg->getNodeColor(i) == "#ffffff"){
                shared_ptr<SwapColorStrat> mcs = make_shared<SwapColorStrat>(cover, i);
                toReturn.push_back(mcs);
            }
        }
    }
    return toReturn;
}

shared_ptr<Strategy> SwapPlayer::getRandomStrat(std::shared_ptr<Game> game) {
    return std::shared_ptr<Strategy>();
}

bool SwapPlayer::isMaximizing() {
    return false;
}

void SwapPlayer::print(ostream &os) const {
    os << "Swap Player";
}

