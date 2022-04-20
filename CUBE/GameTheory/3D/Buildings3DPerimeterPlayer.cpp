#include "Buildings3DPerimeterPlayer.h"
#include "../UrbanBlockGame.h"
#include "Building3DInvariant.h"
#include "../Strategies/DeflateStrat.h"
#include "Inflate3DStrat.h"

#include <utility>

using namespace std;

Buildings3DPerimeterPlayer::Buildings3DPerimeterPlayer(const unsigned int &minum,
                                                       const unsigned int &maxnum,
                                                       const unsigned int &rb,
                                                       const unsigned int &rg,
                                                       const double &angle,
                                                       const double &proj_height)
                                           : Player(),  minum(minum), maxnum(maxnum), rb(rb), rg(rg), angle(angle)
                                           , projection_height(proj_height) {}

Buildings3DPerimeterPlayer::Buildings3DPerimeterPlayer(const unsigned int &minum,
                                                       const unsigned int &maxnum,
                                                       const unsigned int &rb,
                                                       const unsigned int &rg,
                                                       const double &angle,
                                                       const double &proj_height,
                                                       std::function<bool(double, double)> f)
                                           : Player(move(f)), minum(minum), maxnum(maxnum), rb(rb), rg(rg), angle(angle),
                                           projection_height(proj_height) {}

double Buildings3DPerimeterPlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);
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

vector<std::shared_ptr<Strategy>> Buildings3DPerimeterPlayer::getAllStrat(std::shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> toReturn;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    for(int x : ubg->componentsIds("#f00000")){
        if(ubg->componentFromId(x, "#f00000")->getPerimeter() <= maxnum) {
            shared_ptr<Inflate3DStrat> is = make_shared<Inflate3DStrat>(x, "#f00000", rb, rg, angle, projection_height);
            toReturn.push_back(is);
        }
        else {
            shared_ptr<DeflateStrat> ds = make_shared<DeflateStrat>(x, "#f00000");
            toReturn.push_back(ds);
        }
    }
    return toReturn;
}

shared_ptr<Strategy> Buildings3DPerimeterPlayer::getRandomStrat(std::shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    vector<int> keys = ubg->componentsIds("#f00000");
    uniform_int_distribution<> distribution(0, keys.size() - 1);

    int k = distribution(gen);

    int idnum = keys[k];

    if(ubg->componentFromId(idnum, "#f00000")->getPerimeter() <= maxnum) {
        shared_ptr<Inflate3DStrat> is = make_shared<Inflate3DStrat>(idnum, "#f00000", rb, rg, angle, projection_height);
        return is;
    }
    else{
        shared_ptr<DeflateStrat> ds = make_shared<DeflateStrat>(idnum, "#f00000");
        return ds;
    }
}


bool Buildings3DPerimeterPlayer::isMaximizing() {
    return false;
}

void Buildings3DPerimeterPlayer::print(ostream &os) const {
    os << "Buildings3DPerimeterPlayer";
}
