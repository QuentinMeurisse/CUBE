#include "Inflate3DStrat.h"
#include "../GameTheoryUtility.h"
#include "Building3DInvariant.h"

#include <utility>

using namespace std;

Inflate3DStrat::Inflate3DStrat(const int &idnum, std::string color, const unsigned int &rb, const unsigned int &rg,
                               const double &angle, const double &project_height) : idnum(idnum), color(std::move(color)){

    radius_constraint = TwoRadiusConstraint(rb, rg);
    height_constraint = HeightConstraint(angle, project_height);

}

void Inflate3DStrat::select(std::shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    deque<unsigned int> visited;

    shared_ptr<GroupOfCells3D> goc = dynamic_pointer_cast<GroupOfCells3D>(ubg->componentFromId(idnum, color));

    set<unsigned int > border = goc->getBorder();
    for (unsigned int k : border){
        for(unsigned int neigh : GameTheoryUtility::faceBall(t->getCells(), k, 1)){
            if(t->getColor(neigh) == "#ffffff" && find(visited.begin(), visited.end(), neigh) == visited.end()){
                shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(neigh, color);
                visited.push_back(neigh);
                if (radius_constraint.validChangeColor(ubg, s)) {
                    auto inv = ubg->getInvariants();
                    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(inv['b']);
                    set<Segment> edges = t->edgesOf(neigh);
                    bool constraint_ok = true;
                    for (auto e_it = edges.begin(); constraint_ok and e_it != edges.end(); e_it++){
                        Segment e = *e_it;
                        auto buildings = binv->getComponents();
                        for (auto b_it = buildings.begin(); constraint_ok and b_it != buildings.end(); b_it++){
                            auto const&[k, b] = *b_it;
                            shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
                            if (b->getId() != idnum){
                                constraint_ok = height_constraint.valid(e, goc, b3d);
                            }
                        }
                    }
                    if (constraint_ok)
                        q.push_back(s);
                }
            }
        }
    }
    for(const shared_ptr<Strategy>& strat : q){
        strat->select(ubg);
    }

}

void Inflate3DStrat::unselect(std::shared_ptr<Game> g) {
    while (!q.empty()){
        shared_ptr<Strategy> s = q.front();
        q.pop_front();
        s->unselect(g);
    }
}

void Inflate3DStrat::print(std::ostream &os) const {
    os << "Inflate3DStrat: idnum= " << idnum << "\n";
    for(const shared_ptr<Strategy>&  s : q){
        os << "\t" << *s << "\n";
    }
}

bool Inflate3DStrat::equal(const Strategy &other) const {
    try {
        const auto& is = dynamic_cast<const Inflate3DStrat&>(other);
        return this->idnum == is.idnum and this->color == is.color;
    } catch (...) {
        return false;
    }
}
