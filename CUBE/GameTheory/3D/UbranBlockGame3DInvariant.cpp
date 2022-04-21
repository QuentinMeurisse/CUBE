#include "UbranBlockGame3DInvariant.h"
#include "GroupOfCells3D.h"

#include <utility>
#include "memory"

using namespace std;

UbranBlockGame3DInvariant::UbranBlockGame3DInvariant(const std::shared_ptr<UrbanBlockGame> &g, std::string color) :
        UbranBlockGameInvariant(g, std::move(color)) {}

std::shared_ptr<GroupOfCells>
UbranBlockGame3DInvariant::createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum, const std::string &c) {
    shared_ptr<SquareTiling> st = dynamic_pointer_cast<SquareTiling>(tiling);
    shared_ptr<GroupOfCells3D> goc = make_shared<GroupOfCells3D>(st, idnum, c);
    goc->increaseHeight(2.8);
    return goc;
}
