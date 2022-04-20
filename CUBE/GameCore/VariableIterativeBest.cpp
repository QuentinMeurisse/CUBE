#include "VariableIterativeBest.h"
#include "Util.h"

using namespace std;
VariableIterativeBest::VariableIterativeBest(const std::vector<std::shared_ptr<Player>> &players,
                                             const std::shared_ptr<Game> &game) : IterativeGameplay(players, game) {}

std::shared_ptr<Strategy> VariableIterativeBest::selectStrat(std::shared_ptr<Player> p) {
    shared_ptr<MultiStratPlayer> msp = dynamic_pointer_cast<MultiStratPlayer>(p);

    function<double (shared_ptr<Strategy>)> evaluate = [&] (shared_ptr<Strategy> const& s){
        s->select(game);
        double toReturn(msp->payoff(game));
        s->unselect(game);
        return toReturn;
    };
    msp->setStratIndex(1);
    while (msp->getStratIndex() <= msp->getMaxIndex()) {
        vector<shared_ptr<Strategy>> strats(move(msp->getAllStrat(game)));
        if (strats.empty()){
            msp->increaseStratIndex();
        }
        else{
           auto [best, best_eval] = optimum<shared_ptr<Strategy>>(strats, evaluate, msp->isMaximizing());
            if (msp->applyBetter(best_eval, msp->payoff(game))){
                return best;
            }
            else{
                msp->increaseStratIndex();
            }
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Player>> VariableIterativeBest::orderPLayers() {
    return players;
}

VariableIterativeBestWithMemory::VariableIterativeBestWithMemory(const vector<std::shared_ptr<Player>> &players,
                                                                 const shared_ptr<Game> &game) : VariableIterativeBest(
        players, game) {}

shared_ptr<Strategy> VariableIterativeBestWithMemory::selectStrat(std::shared_ptr<Player> p) {
    shared_ptr<MultiStratPlayer> msp = dynamic_pointer_cast<MultiStratPlayer>(p);

    function<double (shared_ptr<Strategy>)> evaluate = [&] (shared_ptr<Strategy> const& s){
        s->select(game);
        double toReturn(msp->payoff(game));
        s->unselect(game);
        return toReturn;
    };
    msp->setStratIndex(1);
    while (msp->getStratIndex() <= msp->getMaxIndex()) {
        vector<shared_ptr<Strategy>> strats(move(msp->getAllStrat(game)));
        if (strats.empty()){
            msp->increaseStratIndex();
        }
        else{
            auto [best, best_eval] = optimum<shared_ptr<Strategy>>(strats, evaluate, msp->isMaximizing());
            if (msp->applyBetter(best_eval, msp->payoff(game))){
                p->addToMemory(best);
                if (p->memoryContainsTooMuch(best))
                    msp->increaseStratIndex();
                else
                    return best;
            }
            else{
                msp->increaseStratIndex();
            }
        }
    }
    return nullptr;
}
