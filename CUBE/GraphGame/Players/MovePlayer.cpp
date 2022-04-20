#include "MovePlayer.h"
#include "../GraphGame.h"
#include "../Invariants/ColorNodeInvariant.h"
#include "../Strats/SwapColorStrat.h"

#include <utility>
#include <map>
#include <limits>
#include <vector>
#include <iostream>

using namespace std;

MovePlayer::MovePlayer(double maxDistance, char key, unsigned int tokenIndex)
        : Player(), maxDistance(maxDistance),
          invariantKey(key), tokenIndex(tokenIndex) {}

MovePlayer::MovePlayer(double maxDistance, char key, unsigned int tokenIndex,
                       std::function<bool(double, double)> better)
        : Player(move(better)),
          invariantKey(key),
          maxDistance(maxDistance),
          tokenIndex(tokenIndex){}

double MovePlayer::payoff(shared_ptr<Game> game) {
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

vector<shared_ptr<Strategy>> MovePlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;

    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> bni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = bni->getTokens();
    uint64_t cover = coverNodes[tokenIndex];
    for(uint64_t succ : g->getSuccessors(cover)){
        if (g->getColor(succ) == "#ffffff"){
            shared_ptr<SwapColorStrat> mcs = make_shared<SwapColorStrat>(cover, succ);
            toReturn.push_back(mcs);
        }
    }

    return toReturn;
}

std::shared_ptr<Strategy> MovePlayer::getRandomStrat(std::shared_ptr<Game> game) {
    return std::shared_ptr<Strategy>();
}

bool MovePlayer::isMaximizing() {
    return false;
}

void MovePlayer::print(std::ostream &os) const {
    os << "Player " << " with token " << tokenIndex << " and color " << invariantKey;
}

CooperativeMovePlayer::CooperativeMovePlayer(double maxDistance,
                                             char key,
                                             unsigned int tokenIndex) : MovePlayer(maxDistance, key, tokenIndex) {}

CooperativeMovePlayer::CooperativeMovePlayer(double maxDistance,
                                             char key,
                                             unsigned int tokenIndex,
                                             const function<bool(double, double)> &better) : MovePlayer(maxDistance,
                                                                                                        key, tokenIndex,
                                                                                                        better) {}

double CooperativeMovePlayer::payoff(std::shared_ptr<Game> game) {
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

CountMovePlayer::CountMovePlayer(double maxDistance,
                                 char key,
                                 unsigned int tokenIndex) : MovePlayer(maxDistance, key, tokenIndex) {}

CountMovePlayer::CountMovePlayer(double maxDistance,
                                 char key,
                                 unsigned int tokenIndex,
                                 const function<bool(double, double)> &better) : MovePlayer(maxDistance, key,
                                                                                            tokenIndex, better) {}

double CountMovePlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(game);
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[invariantKey]);
    vector<uint64_t> coverNodes = cni->getTokens();
    uint64_t player_node = coverNodes[tokenIndex];
    size_t numNodes = gg->numNodes();
    double count = 0;
    for (int i = 0; i < numNodes; ++i) {
        double mindist = numeric_limits<double_t >::infinity();
        vector<uint64_t> closest_nodes;
        for (uint64_t covered : coverNodes){
            size_t cover_index = g->findNodeIndex(covered);
            double  d = gg->distance(i, cover_index);
            if (d < mindist) {
                closest_nodes.clear();
                mindist = d;
                closest_nodes.push_back(covered);
            }
            else if (d == mindist)
                closest_nodes.push_back(covered);
        }
        if (find(closest_nodes.begin(), closest_nodes.end(), player_node) != closest_nodes.end()){
            double factor = mindist <= maxDistance ? 10. : 1.;
            count += factor * 1./static_cast<double>(closest_nodes.size());
        }
    }
    return count;
}

bool CountMovePlayer::isMaximizing() {
    return true;
}

KCenterPlayer::KCenterPlayer(char key, unsigned int tokenIndex): MovePlayer(0, key, tokenIndex) {}

KCenterPlayer::KCenterPlayer(char key, int tokenIndex, const function<bool(double, double)> &better) :
MovePlayer(0, key, tokenIndex, better) {}

double KCenterPlayer::payoff(std::shared_ptr<Game> game) {
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

bool KCenterPlayer::isMaximizing() {
    return false;
}


