#include "PorosityPlayer.h"
#include "../UrbanBlockGame.h"
#include "../Invariants/BuildingInvariant.h"
#include "../Strategies/ChangeColorStrat.h"

#include <memory>
#include <map>
#include <iostream>
#include <random>
#include <utility>

using namespace std;

PorosityPlayer::PorosityPlayer(unsigned int rb, unsigned int rg):Player(){
    constraint = TwoRadiusConstraint(rb, rg);
}

PorosityPlayer::PorosityPlayer(unsigned int rb, unsigned int rg, std::function<bool(double, double)> f):Player(move(f)){
    constraint = TwoRadiusConstraint(rb, rg);
}

double PorosityPlayer::payoff(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<BuildingInvariant> binv = dynamic_pointer_cast<BuildingInvariant>(inv['b']);
    return 1. - (binv->getArea() / ubg->getT()->totalArea());
}

vector<shared_ptr<Strategy>> PorosityPlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();
    for (int i = 0; i < t->numCells(); ++i) {
        if(t->getColor(i) == "#ffffff"){
            shared_ptr<ChangeColorStrat> strat = make_shared<ChangeColorStrat>(i, "#f00000");
            if (constraint.validChangeColor(ubg, strat))
                toReturn.push_back(strat);
        }
    }
    return toReturn;
}

std::shared_ptr<Strategy> PorosityPlayer::getRandomStrat(std::shared_ptr<Game> game) {
    shared_ptr<ChangeColorStrat> toReturn;

    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();

    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    int k;
    int i = 0;
    do{
        k = distribution(gen);
        toReturn = make_shared<ChangeColorStrat>(k, "#f00000");
        i++;
    }while(i <= t->numCells() && (t->getColor(k) == "#f00000" || !constraint.validChangeColor(ubg, toReturn)));
    /* To avoid infinite loop we count the number of iterations. As we generate integers with the uniform distribution
     * on {0, 1, ..., numCells - 1}, if we generate numCells numbers, we tend to generate all the numbers in
     * {0, 1, ..., numCells - 1}. That means that we have test all the strategies but no ones are valid.
     * So we can stop and return a null strategy.
     */
    if (i <= t->numCells())
        return toReturn;
    else
        return nullptr;
}

bool PorosityPlayer::isMaximizing() {
    return false;
}

void PorosityPlayer::print(std::ostream &os) const {
    os << "Porosity";
}


