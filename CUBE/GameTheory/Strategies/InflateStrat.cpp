#include "InflateStrat.h"
#include "../UrbanBlockGame.h"
#include "../GroupOfCells.h"
#include "../GameTheoryUtility.h"
#include "ChangeColorStrat.h"
#include "ChangeBlockColorStrat.h"

#include <memory>
#include <deque>
#include <algorithm>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>

using namespace std;

InflateStrat::InflateStrat(int const &idnum, std::string color, unsigned int const &rb, unsigned int const &rg)
        : idnum(idnum), color(std::move(color)) {
    constraint = TwoRadiusConstraint(rb, rg);
}

void InflateStrat::select(shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    deque<unsigned int> visited;

    shared_ptr<GroupOfCells> goc = ubg->componentFromId(idnum, color);

    set<unsigned int > border = goc->getBorder();
    for (unsigned int k : border){
        for(unsigned int neigh : GameTheoryUtility::faceBall(t->getCells(), k, 1)){
            if(t->getColor(neigh) == "#ffffff" && find(visited.begin(), visited.end(), neigh) == visited.end()){
                shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(neigh, color);
                visited.push_back(neigh);
                //cout << *s << endl;
                if (constraint.validChangeColor(ubg, s))
                    q.push_back(s);
            }
        }
    }
    //cout << "\n\n\n\tapply" << endl;
    for(const shared_ptr<Strategy>& strat : q){
        //cout << "\t" << *strat << endl;
        strat->select(ubg);
    }
}

void InflateStrat::unselect(shared_ptr<Game> g) {
    while (!q.empty()){
        shared_ptr<Strategy> s = q.front();
        q.pop_front();
        s->unselect(g);
    }
}

void InflateStrat::print(ostream &os) const {
    os << "InflateStrat: idnum= " << idnum << "\n";
    for(const shared_ptr<Strategy>&  s : q){
        os << "\t" << *s << "\n";
    }
}

bool InflateStrat::equal(const Strategy &other) const {
    try {
        const auto& is = dynamic_cast<const InflateStrat&>(other);
        return this->idnum == is.idnum and this->color == is.color;
    } catch (...) {
        return false;
    }
}
