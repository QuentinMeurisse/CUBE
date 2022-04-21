#include "UbranBlockGameInvariant.h"
#include "../Strategies/ChangeBlockColorStrat.h"

#include <utility>
#include <iostream>

using namespace std;

UbranBlockGameInvariant::UbranBlockGameInvariant(const shared_ptr<UrbanBlockGame> &g, string color) : Invariant(g), color(move(color)) {}


void UbranBlockGameInvariant::update(const std::shared_ptr<Strategy> &strat) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    shared_ptr<Tiling> t = ubg->getT();
    shared_ptr<ChangeColorStrat> s = dynamic_pointer_cast<ChangeColorStrat>(strat);
    if (s->getNewColor() == color){ // Color of our component
        if(s->getExpanded() == 0 and components.find(s->getNewComponent()) == components.end()){
            // The 2nd condition is here to manage the case we are reconstructing a building without any adjacent cell
            shared_ptr<GroupOfCells> b = createGroupOfCells(t, s->getNewComponent(), s->getNewColor());
            b->addCell(s->getK());
            area += b->getArea();
            perimeter += b->getPerimeter();
            numComponents++;
            components[s->getNewComponent()] = b;
        }
        else{
            shared_ptr<GroupOfCells> b = components[s->getExpanded() | s->getNewComponent()];
            /* If getExpanded == 0, the | operator returns getNewComponent because getNewComponent is already
             * a key of building (we are reconstructing cells by cells a building so we have guarantees that cells are
             * adjacent.
             * The case getExpanded != 0 and newComponent isn't a key can never occur. Because in the definition of
             * ChangeColorStrat, if expanded != 0, newComponent = expanded. And if expanded != 0, there is a building
             * with this value as key.
             * If getExpanded != 0 and getNewComponent is a key, as explained, getExpanded == getNexComponent, so
             * the operator | doesn't change this value.*/
            double oldArea = b->getArea();
            double oldPerimeter = b->getPerimeter();
            b->addCell(s->getK());
            perimeter += (b->getPerimeter() - oldPerimeter);
            area += (b->getArea() - oldArea);
        }
    }
    else if (components.find(s->getOldComponent()) !=components.end()){
        // If we are removing a cell from a building
        shared_ptr<GroupOfCells> b = components[s->getOldComponent()];
        double oldArea = b->getArea();
        double oldPerimeter = b->getPerimeter();
        b->removeCell(s->getK());
        area += (b->getArea() - oldArea);
        perimeter += (b->getPerimeter() - oldPerimeter);
        if (b->getArea() <= 1e-10){
            numComponents--;
            t->releaseId(b->getId());
            components.erase(b->getId());
        }
    }

}

void UbranBlockGameInvariant::undo(const std::shared_ptr<Strategy> &strat) {
    shared_ptr<ChangeColorStrat> s = dynamic_pointer_cast<ChangeColorStrat>(strat);
    shared_ptr<ChangeColorStrat> reversed = make_shared<ChangeColorStrat>(s->reverse());
    update(reversed);
}


double UbranBlockGameInvariant::getArea() const {
    return area;
}

double UbranBlockGameInvariant::getPerimeter() const {
    return perimeter;
}

unsigned int UbranBlockGameInvariant::getNumComponents() const {
    return numComponents;
}

const map<int, std::shared_ptr<GroupOfCells>> &UbranBlockGameInvariant::getComponents() const {
    return components;
}

std::shared_ptr<GroupOfCells>
UbranBlockGameInvariant::createGroupOfCells(const shared_ptr<Tiling> &tiling, unsigned int idnum, const string &c) {
    shared_ptr<GroupOfCells> goc = make_shared<GroupOfCells>(tiling, idnum, c);
    return goc;
}
