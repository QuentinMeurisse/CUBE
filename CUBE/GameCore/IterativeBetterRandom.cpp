#include "IterativeBetterRandom.h"
#include "Timer.h"
#include "Util.h"

#include <chrono>
#include <sstream>
#include <memory>
#include <iostream>
#include <limits>

using namespace std;
using namespace std::chrono;

IterativeBetterRandom::IterativeBetterRandom(const std::vector<std::shared_ptr<Player>> &players,
                                             const std::shared_ptr<Game> &game,
                                             unsigned int maxTimeWithoutImprovement, unsigned int maxTotalTime) :
                                             IterativeGameplay(players, game),
                                             maxTimeWithoutImprovement(maxTimeWithoutImprovement),
                                             maxTotalTime(maxTotalTime){}


void IterativeBetterRandom::play() {
    for(auto & l : listeners){
        l->start(*this);
    }
    Timer improveTimer(maxTimeWithoutImprovement);
    Timer totalTimer(maxTotalTime);
    totalTimer.start();
    while (not improveTimer.timeElapsed() and not totalTimer.timeElapsed() and not stopFlag){
        vector<shared_ptr<Player>> ordered = orderPLayers();
        bool improvement = false;
        for(auto & p : ordered){
            shared_ptr<Strategy> strat(selectStrat(p));
            ostringstream ss;
            if (strat != nullptr) {
                ss << *strat;
                strat->select(game);
                improvement = true;
            }
            else{
                ss << "None";
            }
            lastStrat = ss.str();
            for(auto & l : listeners){
                l->step(*this, *p);
            }
        }
        if(!improvement)
            improveTimer.start();
        else
            improveTimer.stop();
    }

    for(auto & l : listeners){
        l->stop(*this);
    }
}

shared_ptr<Strategy> IterativeBetterRandom::selectStrat(shared_ptr<Player> p) {
    shared_ptr<Strategy> s = p->getRandomStrat(game);

    if (s == nullptr)
        return s;
    else {
        s->select(game);
        double newPayoff(p->payoff(game));
        s->unselect(game);

        if (p->applyBetter(newPayoff, p->payoff(game)))
            return s;
        else
            return nullptr;
    }
}

vector<shared_ptr<Player>> IterativeBetterRandom::orderPLayers() {
    return players;
}
