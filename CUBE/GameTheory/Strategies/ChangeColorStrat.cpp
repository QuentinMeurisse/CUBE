#include "ChangeColorStrat.h"
#include "../Tiling.h"
#include "../UrbanBlockGame.h"

#include <utility>
#include <memory>
#include <iostream>

using namespace std;

ChangeColorStrat::ChangeColorStrat(unsigned int k, string newColor) : k(k), newColor(move(newColor)) {}

void ChangeColorStrat::select(shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    oldColor = t->getColor(k);
    oldComponent = t->getComponent(k);
    expanded = t->closerComponent(k, newColor);
    if (expanded == 0)
        newComponent = t->nextId();
    else
        newComponent = expanded;

    shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(*this);
    ubg->updateAll(s);

    t->setColor(k, newColor);
    if(newColor == "#ffffff")
        t->setComponent(k, 0);
    else
        t->setComponent(k, newComponent);
}

void ChangeColorStrat::unselect(shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    oldExpanded = t->closerComponent(k, oldColor);
    if(oldComponent != 0 && !ubg->doTheComponentExist(oldComponent, oldColor)){
    // We will delete a whole component. So its id can be reused. We need to release it now.
        t->nextId();
    }
    shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(*this);
    ubg->undoAll(s);
    t->setColor(k, oldColor);
    t->setComponent(k, oldComponent);
}

unsigned int ChangeColorStrat::getK() const {
    return k;
}

const string &ChangeColorStrat::getNewColor() const {
    return newColor;
}

const string &ChangeColorStrat::getOldColor() const {
    return oldColor;
}

unsigned int ChangeColorStrat::getExpanded() const {
    return expanded;
}

unsigned int ChangeColorStrat::getOldExpanded() const {
    return oldExpanded;
}

unsigned int ChangeColorStrat::getNewComponent() const {
    return newComponent;
}

unsigned int ChangeColorStrat::getOldComponent() const {
    return oldComponent;
}

ChangeColorStrat ChangeColorStrat::reverse() {
    ChangeColorStrat reversed(this->k, this->oldColor);
    reversed.oldColor = this->newColor;
    reversed.oldComponent = this->newComponent;
    reversed.newComponent = this->oldComponent;
    reversed.expanded = this->oldExpanded;
    reversed.oldExpanded = this->expanded;
    return reversed;
}

void ChangeColorStrat::print(std::ostream &os) const {
    os << "ChangeColorStrat:\tk = " << k << "\tcolor = " << newColor;
}

bool ChangeColorStrat::equal(const Strategy &other) const {
    try {
        const auto& ccs = dynamic_cast<const ChangeColorStrat&>(other);
        return this->k == ccs.k and this->newColor == ccs.newColor;
    } catch (...) {
        return false;
    }
}
