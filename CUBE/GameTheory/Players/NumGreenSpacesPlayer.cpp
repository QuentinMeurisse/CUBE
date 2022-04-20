#include "NumGreenSpacesPlayer.h"
#include "../UrbanBlockGame.h"
#include "Invariant.h"
#include "../Invariants/GreenSpaceInvariant.h"
#include "../Strategies/ChangeBlockColorStrat.h"


#include <memory>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

NumGreenSpacesPlayer::NumGreenSpacesPlayer(unsigned int const &minnum,
                                           unsigned int const &maxnum,
                                           unsigned int const &rb,
                                           unsigned int const &rg) :Player(), minnum(minnum), maxnum(maxnum){
    constraint = TwoRadiusConstraint(rb, rg);
}

NumGreenSpacesPlayer::NumGreenSpacesPlayer(unsigned int const &minnum, unsigned int const &maxnum,
                                           unsigned int const &rb, unsigned int const &rg,
                                           std::function<bool(double, double)> f): Player(move(f)), minnum(minnum), maxnum(maxnum) {

    constraint = TwoRadiusConstraint(rb, rg);
}

double NumGreenSpacesPlayer::payoff(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<GreenSpaceInvariant> ginv = dynamic_pointer_cast<GreenSpaceInvariant>(inv['g']);
    if (ginv->getNumComponents() < minnum)
        return minnum - ginv->getNumComponents();
    else if (ginv->getNumComponents() > maxnum)
        return ginv->getNumComponents() - maxnum;
    else
        return 0.;

}

vector<shared_ptr<Strategy>> NumGreenSpacesPlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    vector<unsigned int > visited;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();

    for (int i = 0; i < t->numCells(); ++i) {
        if (t->getColor(i) == "#ffffff"){
            if (t->closerComponent(i, "#00f000") == 0){
                shared_ptr<ChangeColorStrat> strat = make_shared<ChangeColorStrat>(i, "#00f000");
                if (constraint.validChangeColor(ubg, strat))
                    toReturn.push_back(strat);
            }
        }
        else if (t->getColor(i) == "#00f000"){
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

shared_ptr<Strategy> NumGreenSpacesPlayer::getRandomStrat(shared_ptr<Game> game) {
    shared_ptr<ChangeColorStrat> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<GreenSpaceInvariant> ginv = dynamic_pointer_cast<GreenSpaceInvariant>(inv['g']);

    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    int k;
    do{
        k = distribution(gen);
        if (t->getColor(k) == "#00f000" and ginv->getNumComponents() > maxnum){
            shared_ptr<ChangeBlockColorStrat> s = make_shared<ChangeBlockColorStrat>(k, "#ffffff");
            return s;
        }
        else{
            toReturn = make_shared<ChangeColorStrat>(k, "#00f000");
        }
    }while (t->getColor(k) != "#ffffff" or !constraint.validChangeColor(ubg, toReturn) or toReturn->getExpanded() != 0);
    return toReturn;
}

bool NumGreenSpacesPlayer::isMaximizing() {
    return false;
}

void NumGreenSpacesPlayer::print(ostream &os) const {
    os << "NumGreenSpacesPlayer";
}

