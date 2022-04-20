#include "GreenSpaces3DInvariant.h"
#include "group_of_cells3D.h"


using namespace std;
GreenSpaces3DInvariant::GreenSpaces3DInvariant(const std::shared_ptr<UrbanBlockGame> &g) : GreenSpaceInvariant(g) {}

std::shared_ptr<GroupOfCells>
GreenSpaces3DInvariant::createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum,
                                           const std::string &c) {
    shared_ptr<SquareTiling> st = dynamic_pointer_cast<SquareTiling>(tiling);
    shared_ptr<GroupOfCells3D> goc = make_shared<GroupOfCells3D>(st, idnum, c);
    return goc;
}
