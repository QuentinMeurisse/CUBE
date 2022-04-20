#include "GreenSpaceRatePlayer.h"
#include "../UrbanBlockGame.h"
#include "Invariant.h"
#include "../Invariants/GreenSpaceInvariant.h"
#include "../Strategies/InflateStrat.h"

#include <memory>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

GreenSpaceRatePlayer::GreenSpaceRatePlayer(double const &ratio, unsigned int const &rb, unsigned int const &rg)
        :Player(), ratio(ratio), rb(rb), rg(rg) {}

GreenSpaceRatePlayer::GreenSpaceRatePlayer(double const &ratio, unsigned int const &rb, unsigned int const &rg,
                                           std::function<bool(double, double)> f):Player(move(f)), ratio(ratio), rb(rb), rg(rg){}

double GreenSpaceRatePlayer::payoff(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<GreenSpaceInvariant> ginv = dynamic_pointer_cast<GreenSpaceInvariant>(inv['g']);
    double x = ginv->getArea() / ubg->getT()->totalArea();
    if (x < ratio)
        return ratio - x;
    else
        return 0.;
}

vector<shared_ptr<Strategy>> GreenSpaceRatePlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    for(int x : ubg->componentsIds("#00f000")){
        shared_ptr<InflateStrat> is = make_shared<InflateStrat>(x, "#00f000", rb, rg);
        toReturn.push_back(is);
    }
    return toReturn;
}

shared_ptr<Strategy> GreenSpaceRatePlayer::getRandomStrat(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    vector<int> keys = ubg->componentsIds("#00f000");
    uniform_int_distribution<> distribution(0, keys.size() - 1);

    int k = distribution(gen);

    int idnum = keys[k];

    shared_ptr<InflateStrat> is = make_shared<InflateStrat>(idnum, "#00f000", rb, rg);
    return is;
}

bool GreenSpaceRatePlayer::isMaximizing() {
    return false;
}

void GreenSpaceRatePlayer::print(ostream &os) const {
    os << "GreenSpaceRatePlayer";

}




