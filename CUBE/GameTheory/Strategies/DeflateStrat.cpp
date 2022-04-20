#include "DeflateStrat.h"
#include "../UrbanBlockGame.h"
#include "../Tiling.h"

#include <utility>
#include <memory>
#include <iostream>

using namespace std;

DeflateStrat::DeflateStrat(int idnum, string color) : idnum(idnum), color(move(color)) {}

void DeflateStrat::select(shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    deque<unsigned int> visited;

    shared_ptr<GroupOfCells> goc = ubg->componentFromId(idnum, color);


    for(unsigned int k : goc->getBorder()){
        shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(k, "#ffffff");
        q.push_back(s);
    }

    for (const shared_ptr<ChangeColorStrat>& s  : q){
        s->select(ubg);
    }
}

void DeflateStrat::unselect(shared_ptr<Game> g) {
    while (!q.empty()){
        shared_ptr<ChangeColorStrat> s = q.front();
        q.pop_front();
        s->unselect(g);
    }
}

void DeflateStrat::print(std::ostream &os) const {
    os << "DeflateStrat: idnum= " << idnum << "\n";
    for(const shared_ptr<ChangeColorStrat>& s : q){
        os << "\t" << *s << "\n";
    }
}

bool DeflateStrat::equal(const Strategy &other) const {
    try {
        const auto& ds = dynamic_cast<const DeflateStrat&>(other);
        return this->idnum == ds.idnum and this->color == ds.color;
    } catch (...) {
        return false;
    }
}
