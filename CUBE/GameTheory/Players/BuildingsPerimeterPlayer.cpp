#include "BuildingsPerimeterPlayer.h"
#include "Invariant.h"
#include "../Invariants/BuildingInvariant.h"
#include "../UrbanBlockGame.h"
#include "../Strategies/InflateStrat.h"
#include "../Strategies/DeflateStrat.h"

#include <memory>
#include <random>
#include <iostream>

using namespace std;

BuildingsPerimeterPlayer::BuildingsPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum,
                                                   unsigned int const &rb,
                                                   unsigned int const &rg) :Player(), minum(minum), maxnum(maxnum), rb(rb), rg(rg){}

BuildingsPerimeterPlayer::BuildingsPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum,
                                                   unsigned int const &rb, unsigned int const &rg,
                                                   std::function<bool(double, double)> f):Player(move(f)),
                                                   minum(minum), maxnum(maxnum), rb(rb), rg(rg){}



double BuildingsPerimeterPlayer::payoff(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<BuildingInvariant> binv = dynamic_pointer_cast<BuildingInvariant>(inv['b']);
    double sum = 0.;
    for (const auto& b : binv->getComponents()){
        double perim = b.second->getPerimeter();
        if(perim < minum)
            sum += (minum - perim);
        if(perim > maxnum)
            sum += (perim - maxnum);
    }
    return sum;
}

vector<shared_ptr<Strategy>> BuildingsPerimeterPlayer::getAllStrat(shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    for(int x : ubg->componentsIds("#f00000")){
        if(ubg->componentFromId(x, "#f00000")->getPerimeter() <= maxnum) {
            shared_ptr<InflateStrat> is = make_shared<InflateStrat>(x, "#f00000", rb, rg);
            toReturn.push_back(is);
        }
        else {
            shared_ptr<DeflateStrat> ds = make_shared<DeflateStrat>(x, "#f00000");
            toReturn.push_back(ds);
        }
    }
    return toReturn;
}

shared_ptr<Strategy> BuildingsPerimeterPlayer::getRandomStrat(shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    vector<int> keys = ubg->componentsIds("#f00000");
    uniform_int_distribution<> distribution(0, keys.size() - 1);

    int k = distribution(gen);

    int idnum = keys[k];

    if(ubg->componentFromId(idnum, "#f00000")->getPerimeter() <= maxnum) {
        shared_ptr<InflateStrat> is = make_shared<InflateStrat>(idnum, "#f00000", rb, rg);

        return is;
    }
    else{
       shared_ptr<DeflateStrat> ds = make_shared<DeflateStrat>(idnum, "#f00000");
        return ds;
    }
}

bool BuildingsPerimeterPlayer::isMaximizing() {
    return false;
}

void BuildingsPerimeterPlayer::print(std::ostream &os) const {
    os << "BuildingsPerimeterPlayer";
}
