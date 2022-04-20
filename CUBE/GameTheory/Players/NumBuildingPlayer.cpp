#include "NumBuildingPlayer.h"
#include "../UrbanBlockGame.h"
#include "../Invariants/BuildingInvariant.h"
#include "../Strategies/ChangeBlockColorStrat.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <fstream>

using namespace std;

NumBuildingPlayer::NumBuildingPlayer(const unsigned int &minnum, const unsigned int &maxnum, unsigned int const &rb,
                                     unsigned int const &rg): Player(), minnum(minnum), maxnum(maxnum) {
    constraint = TwoRadiusConstraint(rb, rg);
}

NumBuildingPlayer::NumBuildingPlayer(const unsigned int &minnum, const unsigned int &maxnum, unsigned int const &rb,
                                     unsigned int const &rg, std::function<bool(double, double)> f): Player(move(f)),
                                     minnum(minnum), maxnum(maxnum){

    constraint = TwoRadiusConstraint(rb, rg);

}

double NumBuildingPlayer::payoff(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<BuildingInvariant> binv = dynamic_pointer_cast<BuildingInvariant>(inv['b']);
    if (binv->getNumComponents() < minnum)
        return minnum - binv->getNumComponents();
    else if (binv->getNumComponents() > maxnum)
        return binv->getNumComponents() - maxnum;
    else
        return 0;
}

vector<shared_ptr<Strategy>> NumBuildingPlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    vector<unsigned int > visited; // Used to avoid multiple ChangeBlockColorStrat that delete the same building
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();

    for (int i = 0; i < t->numCells(); ++i) {
        if (t->getColor(i) == "#ffffff"){
            if (t->closerComponent(i, "#f00000") == 0){
                shared_ptr<ChangeColorStrat> strat = make_shared<ChangeColorStrat>(i, "#f00000");
                if (constraint.validChangeColor(ubg, strat))
                    toReturn.push_back(strat);
            }
        }
        else if (t->getColor(i) == "#f00000"){
            unsigned int currentComponent = t->getComponent(i);
            auto it = find(visited.begin(), visited.end(), currentComponent);
            if (it == visited.end()){
                shared_ptr<ChangeBlockColorStrat> s = make_shared<ChangeBlockColorStrat>(i, "#ffffff");
                toReturn.push_back(s);
                visited.push_back(currentComponent);
            }
        }
    }
    return toReturn;
}

shared_ptr<Strategy> NumBuildingPlayer::getRandomStrat(shared_ptr<Game> game) {
    shared_ptr<ChangeColorStrat> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<BuildingInvariant> binv = dynamic_pointer_cast<BuildingInvariant>(inv['b']);

    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    int k;
    do{
        k = distribution(gen);
        if (t->getColor(k) == "#f00000" and binv->getNumComponents() > maxnum){
            shared_ptr<ChangeBlockColorStrat> s = make_shared<ChangeBlockColorStrat>(k, "#ffffff");
            return s;
        }
        else{
            toReturn = make_shared<ChangeColorStrat>(k, "#f00000");
        }
    }while (t->getColor(k) != "#ffffff" or !constraint.validChangeColor(ubg, toReturn) or toReturn->getExpanded() != 0);
    return toReturn;
}

bool NumBuildingPlayer::isMaximizing() {
    return false;
}

void NumBuildingPlayer::print(ostream &os) const {
    os << "NumBuildingPlayer";
}
