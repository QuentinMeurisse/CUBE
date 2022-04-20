#include "HeightPlayer.h"
#include "../UrbanBlockGame.h"
#include "Building3DInvariant.h"
#include "GroupOfCells3D.h"
#include "IncreaseHeightStrat.h"

#include <memory>

using namespace std;
HeightPlayer::HeightPlayer(double floor, double angle, double height_projection) : floor(floor) {

    constraint = HeightConstraint(angle, height_projection);
}

HeightPlayer::HeightPlayer(double floor,
                           double angle,
                           double height_projection,
                           const std::function<bool(double, double)> &better) : Player(better), floor(floor) {
    constraint = HeightConstraint(angle, height_projection);
}

double HeightPlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    auto inv = ubg->getInvariants();
    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);
    auto buildings = binv->getComponents();
    double sum_of_height = 0;
    for (auto const& [k, b] : buildings){
        shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
        if (b3d->getHeight() < 100)
            sum_of_height += b3d->getHeight();
    }
    return sum_of_height;
}

std::vector<std::shared_ptr<Strategy>> HeightPlayer::getAllStrat(std::shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> to_return;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    auto buildings_ids = ubg->componentsIds("#f00000");
    for (int x : buildings_ids){
        shared_ptr<IncreaseHeightStrat> strat = make_shared<IncreaseHeightStrat>(x, floor);
        strat->select(ubg);
        bool constraint_ok = true;
        auto b1 = ubg->componentFromId(x, "#f00000");
        shared_ptr<GroupOfCells3D> b1_3d = dynamic_pointer_cast<GroupOfCells3D>(b1);
        for (auto it = buildings_ids.begin(); it != buildings_ids.end() and constraint_ok; it++){
            int y = *it;
            if (y != x){
                auto b2 = ubg->componentFromId(y, "#f00000");
                shared_ptr<GroupOfCells3D> b2_3d = dynamic_pointer_cast<GroupOfCells3D>(b2);
                constraint_ok = constraint.valid(b1_3d, b2_3d);
            }
        }
        strat->unselect(ubg);
        if (constraint_ok)
            to_return.push_back(strat);
    }

    return to_return;
}

std::shared_ptr<Strategy> HeightPlayer::getRandomStrat(std::shared_ptr<Game> game) {
    shared_ptr<IncreaseHeightStrat> to_return;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);

    vector<int> keys = ubg->componentsIds("#f00000");
    vector<int> buildings_ids = ubg->componentsIds("#f00000");

    int index;
    bool constraint_ok;
    do {
        constraint_ok = true;
        if (not keys.empty()) {
            uniform_int_distribution<> distribution(0, keys.size() - 1);
            index = distribution(gen);
            int idnum = keys[index];
            keys.erase(keys.begin() + index);
            to_return = make_shared<IncreaseHeightStrat>(idnum, floor);
            to_return->select(ubg);
            auto b1 = ubg->componentFromId(idnum, "#f00000");
            shared_ptr<GroupOfCells3D> b1_3d = dynamic_pointer_cast<GroupOfCells3D>(b1);
            for (auto it = buildings_ids.begin(); it != buildings_ids.end() and constraint_ok; it++){
                int y = *it;
                if (y != idnum){
                    auto b2 = ubg->componentFromId(y, "#f00000");
                    shared_ptr<GroupOfCells3D> b2_3d = dynamic_pointer_cast<GroupOfCells3D>(b2);
                    constraint_ok = constraint.valid(b1_3d, b2_3d);
                }
            }
            to_return->unselect(ubg);
        }
        else {
            uniform_int_distribution<> distribution(0, buildings_ids.size() - 1);
            index = distribution(gen);
            int idnum = buildings_ids[index];
            to_return = make_shared<IncreaseHeightStrat>(idnum, 0);
        }

    } while (not constraint_ok);

    return to_return;
}

bool HeightPlayer::isMaximizing() {
    return true;
}

void HeightPlayer::print(std::ostream &os) const {
    os << "HeightPlayer";
}
