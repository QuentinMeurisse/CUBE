#include "NumBuilding3DPlayer.h"
#include "Building3DInvariant.h"
#include "../Strategies/ChangeColorStrat.h"
#include "../Strategies/ChangeBlockColorStrat.h"
#include <utility>

using namespace std;

NumBuilding3DPlayer::NumBuilding3DPlayer(const unsigned int &minnum,
                                         const unsigned int &maxnum,
                                         const unsigned int &rb,
                                         const unsigned int &rg,
                                         const double &angle,
                                         const double &height_proj) : Player(), minnum(minnum), maxnum(maxnum) {
    radius_constraint = TwoRadiusConstraint(rb, rg);
    height_constraint = HeightConstraint(angle, height_proj);
}

NumBuilding3DPlayer::NumBuilding3DPlayer(const unsigned int &minnum,
                                         const unsigned int &maxnum,
                                         const unsigned int &rb,
                                         const unsigned int &rg,
                                         const double &angle,
                                         const double &height_proj,
                                         function<bool(double, double)> f)
                                         :Player(move(f)), minnum(minnum), maxnum(maxnum) {

    radius_constraint = TwoRadiusConstraint(rb, rg);
    height_constraint = HeightConstraint(angle, height_proj);
}

double NumBuilding3DPlayer::payoff(std::shared_ptr<Game> game) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);
    if (binv->getNumComponents() < minnum)
        return minnum - binv->getNumComponents();
    else if (binv->getNumComponents() > maxnum)
        return binv->getNumComponents() - maxnum;
    else
        return 0;
}

vector<std::shared_ptr<Strategy>> NumBuilding3DPlayer::getAllStrat(std::shared_ptr<Game> game) {
    vector<shared_ptr<Strategy>> to_return;
    vector<unsigned int > visited; // Used to avoid multiple ChangeBlockColorStrat that delete the same building
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();

    for (int i = 0; i < t->numCells(); ++i) {
        if (t->getColor(i) == "#ffffff"){
            if (t->closerComponent(i, "#f00000") == 0){
                shared_ptr<ChangeColorStrat> strat = make_shared<ChangeColorStrat>(i, "#f00000");
                if (radius_constraint.validChangeColor(ubg, strat)){
                    strat->select(ubg);
                    uint new_id = strat->getNewComponent();
                    auto inv = ubg->getInvariants();
                    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);
                    auto buildings = binv->getComponents();
                    shared_ptr<GroupOfCells3D> new_b = dynamic_pointer_cast<GroupOfCells3D>(buildings[new_id]);
                    bool height_constraint_ok = true;
                    for (auto b_it = buildings.begin(); height_constraint_ok and b_it != buildings.end(); b_it++){
                        auto const&[k, b] = *b_it;
                        if (k != new_id){
                            shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
                            height_constraint_ok = height_constraint.valid(new_b, b3d);
                        }
                    }
                    strat->unselect(ubg);
                    if (height_constraint_ok)
                        to_return.push_back(strat);
                }

            }
        }
        else if (t->getColor(i) == "#f00000"){
            unsigned int currentComponent = t->getComponent(i);
            auto it = find(visited.begin(), visited.end(), currentComponent);
            if (it == visited.end()){
                shared_ptr<ChangeBlockColorStrat> s = make_shared<ChangeBlockColorStrat>(i, "#ffffff");
                to_return.push_back(s);
                visited.push_back(currentComponent);
            }
        }
    }
    return to_return;
}

shared_ptr<Strategy> NumBuilding3DPlayer::getRandomStrat(std::shared_ptr<Game> game) {
    shared_ptr<ChangeColorStrat> to_return;
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    shared_ptr<Tiling> t = ubg->getT();
    map<char, shared_ptr<Invariant>> inv = ubg->getInvariants();
    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);

    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    int k;
    bool constraints_ok;
    do{
        constraints_ok = true;
        k = distribution(gen);
        if (t->getColor(k) == "#f00000" and binv->getNumComponents() > maxnum){
            shared_ptr<ChangeBlockColorStrat> s = make_shared<ChangeBlockColorStrat>(k, "#ffffff");
            return s;
        }
        else{
            to_return = make_shared<ChangeColorStrat>(k, "#f00000");
            if (t->getColor(k) != "#ffffff"
                or !radius_constraint.validChangeColor(ubg, to_return)
                or to_return->getExpanded() != 0) {
                constraints_ok = false;
            }
            else{
                to_return->select(ubg);
                uint new_id = to_return->getNewComponent();
                auto buildings = binv->getComponents();
                shared_ptr<GroupOfCells3D> new_b = dynamic_pointer_cast<GroupOfCells3D>(buildings[new_id]);
                for (auto b_it = buildings.begin(); constraints_ok and b_it != buildings.end(); b_it++){
                    auto const&[k, b] = *b_it;
                    if (k != new_id){
                        shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
                        constraints_ok = height_constraint.valid(new_b, b3d);
                    }
                }
                to_return->unselect(ubg);
            }
        }
    }while (not constraints_ok);
    return to_return;
}

bool NumBuilding3DPlayer::isMaximizing() {
    return false;
}

void NumBuilding3DPlayer::print(ostream &os) const {
    os << "NumBuilding3DPlayer";
}
