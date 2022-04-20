#include "IncreaseHeightStrat.h"
#include "../UrbanBlockGame.h"
#include "Building3DInvariant.h"
#include "GroupOfCells3D.h"


using namespace std;
IncreaseHeightStrat::IncreaseHeightStrat(const int &idnum, const double &floor) : idnum(idnum), floor(floor) {}

void IncreaseHeightStrat::select(std::shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    auto b = ubg->componentFromId(idnum, "#f00000");
    shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
    b3d->increaseHeight(floor);
}

void IncreaseHeightStrat::unselect(std::shared_ptr<Game> g) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(g);
    auto b = ubg->componentFromId(idnum, "#f00000");
    shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
    b3d->decreaseHeight(floor);
}

void IncreaseHeightStrat::print(std::ostream &os) const {
    os << "IncreaseHeightStrat idnum= " << idnum << endl;

}

bool IncreaseHeightStrat::equal(const Strategy &other) const {
    try {
        const auto& is = dynamic_cast<const IncreaseHeightStrat&>(other);
        return this->idnum == is.idnum and this->floor == is.floor;
    } catch (...) {
        return false;
    }
}

