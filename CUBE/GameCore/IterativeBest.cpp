#include "IterativeBest.h"
#include "Util.h"
#include "FiniteMemory.h"
#include <functional>
#include <utility>
#include <iostream>

using namespace std;

IterativeBest::IterativeBest(const vector<std::shared_ptr<Player>> &players, const shared_ptr<Game> &game)
        : IterativeGameplay(players, game) {}

std::shared_ptr<Strategy> IterativeBest::selectStrat(shared_ptr<Player> p) {
    vector<shared_ptr<Strategy>> strats(move(p->getAllStrat(game)));

    function<double (shared_ptr<Strategy>)> evaluate = [&] (shared_ptr<Strategy> const& s){
        s->select(game);
        double toReturn(p->payoff(game));
        s->unselect(game);
        return toReturn;
    };

    if (strats.empty()){
        return nullptr;
    }
    else{
        pair<shared_ptr<Strategy>, double > best = optimum<shared_ptr<Strategy>>(strats, evaluate, p->isMaximizing());
        if (p->applyBetter(best.second, p->payoff(game))){
            return best.first;
        }
        else{
            return nullptr;
        }
    }
}

std::vector<std::shared_ptr<Player>> IterativeBest::orderPLayers() {
    return players;
}


IterativeBestWithMemory::IterativeBestWithMemory(const vector<std::shared_ptr<Player>> &players,
                                                 const shared_ptr<Game> &game) : IterativeBest(players, game) {}

shared_ptr<Strategy> IterativeBestWithMemory::selectStrat(std::shared_ptr<Player> p) {
    vector<shared_ptr<Strategy>> strats(move(p->getAllStrat(game)));

    function<double (shared_ptr<Strategy>)> evaluate = [&] (shared_ptr<Strategy> const& s){
        s->select(game);
        double toReturn(p->payoff(game));
        s->unselect(game);
        return toReturn;
    };

    if (strats.empty()){
        return nullptr;
    }
    else{
        auto [best, best_eval] = optimum<shared_ptr<Strategy>>(strats, evaluate, p->isMaximizing());
        if (p->applyBetter(best_eval, p->payoff(game))){
            p->addToMemory(best);
            if (p->memoryContainsTooMuch(best))
                return nullptr;
            else
                return best;
        }
        else{
            return nullptr;
        }
    }
}
