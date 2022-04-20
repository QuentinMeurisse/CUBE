#include "IterativeGameplay.h"

#include <utility>
#include <iostream>
#include <sstream>

using namespace std;

IterativeGameplay::IterativeGameplay(vector<std::shared_ptr<Player>> players, shared_ptr<Game> game)
        : players(std::move(players)), game(std::move(game)) {}

IterativeGameplay::~IterativeGameplay() = default;

void IterativeGameplay::play() {
    for(auto & l : listeners){
        l->start(*this);
    }
    bool improvement;
    do{
        vector<shared_ptr<Player>> ordered = orderPLayers();
        improvement = false;
        for(auto & p : ordered){
            shared_ptr<Strategy> strat(selectStrat(p));
            ostringstream ss;
            if (strat != nullptr) {
                ss << *strat;
                strat->select(game);
                improvement = true ;
            }
            else{
                ss << "None";
            }
            lastStrat = ss.str();
            for(auto & l : listeners){
                l->step(*this, *p);
            }
        }
    }while (improvement and not stopFlag);

    for(auto & l : listeners){
        l->stop(*this);
    }

}

void IterativeGameplay::addListener(shared_ptr<Listener> const& l) {
    listeners.push_back(l);
}

const shared_ptr<Game> &IterativeGameplay::getGame() const {
    return game;
}

const vector<std::shared_ptr<Player>> &IterativeGameplay::getPlayers() const {
    return players;
}

const string &IterativeGameplay::getLastStrat() const {
    return lastStrat;
}

void IterativeGameplay::stop() {
    stopFlag = true;

}
