#ifndef CUBE_GREENSPACES3DINVARIANT_H
#define CUBE_GREENSPACES3DINVARIANT_H

#include "../Invariants/GreenSpaceInvariant.h"

class GreenSpaces3DInvariant : public GreenSpaceInvariant {

public:
    explicit GreenSpaces3DInvariant(const std::shared_ptr<UrbanBlockGame> &g);

    std::shared_ptr<GroupOfCells>
    createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum, const std::string &c) override;


};


#endif //CUBE_GREENSPACES3DINVARIANT_H
