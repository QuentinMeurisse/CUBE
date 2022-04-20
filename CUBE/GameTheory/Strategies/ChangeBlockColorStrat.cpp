#include "ChangeBlockColorStrat.h"
#include "../UrbanBlockGame.h"
#include "../GroupOfCells.h"

#include <utility>
#include <memory>
#include <optional>
#include <iostream>

using namespace std;

ChangeBlockColorStrat::ChangeBlockColorStrat(unsigned int const& k, string newColor) : k(k), newColor(move(newColor)){}

void ChangeBlockColorStrat::select(shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    optional<shared_ptr<GroupOfCells>> optgoc = ubg->getGroupOfCells(k);
    if (!optgoc.has_value()){
        shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(k, newColor);
        q.push_back(s);
        s->select(g);
    }
    else{
        shared_ptr<GroupOfCells> goc = optgoc.value();
        for (unsigned int const num : goc->getCells()){
            shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(num, newColor);
            q.push_back(s);
        }
        for (const shared_ptr<ChangeColorStrat>& s : q)
            s->select(g);
    }
}

void ChangeBlockColorStrat::unselect(shared_ptr<Game> g) {
    while (!q.empty()){
        shared_ptr<ChangeColorStrat> s = q.front();
        q.pop_front();
        s->unselect(g);
    }
}

void ChangeBlockColorStrat::print(std::ostream &os) const {
    os << "ChangeBlockColorStrat: k= " << k << " newColor = " << newColor << "\n";
    for(const shared_ptr<ChangeColorStrat>&  s : q){
        os << "\t" << *s << "\n";
    }
}

bool ChangeBlockColorStrat::equal(const Strategy &other) const {
    try {
        const auto& cbcs = dynamic_cast<const ChangeBlockColorStrat&>(other);
        return this->k == cbcs.k and this->newColor == cbcs.newColor;
    } catch (...) {
        return false;
    }
}
