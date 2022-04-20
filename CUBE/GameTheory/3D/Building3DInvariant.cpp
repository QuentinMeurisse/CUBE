#include "Building3DInvariant.h"
#include "../SquareTiling.h"
#include "group_of_cells3D.h"

#include <memory>

using namespace std;
Building3DInvariant::Building3DInvariant(const std::shared_ptr<UrbanBlockGame> &g, double floor)
: BuildingInvariant(g), floor_height(floor) {}

std::shared_ptr<GroupOfCells>
Building3DInvariant::createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum,
                                        const std::string &c) {
    shared_ptr<SquareTiling> st = dynamic_pointer_cast<SquareTiling>(tiling);
    shared_ptr<GroupOfCells3D> goc = make_shared<GroupOfCells3D>(st, idnum, c);
    goc->increaseHeight(floor_height);
    return goc;
}
