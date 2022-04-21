#ifndef CUBE_UBRANBLOCKGAME3DINVARIANT_H
#define CUBE_UBRANBLOCKGAME3DINVARIANT_H

#include "../Invariants/UbranBlockGameInvariant.h"

class UbranBlockGame3DInvariant : public UbranBlockGameInvariant{
public:
    UbranBlockGame3DInvariant(const std::shared_ptr<UrbanBlockGame> &g, std::string color);

    std::shared_ptr<GroupOfCells>
    createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum, const std::string &c) override;
};


#endif //CUBE_UBRANBLOCKGAME3DINVARIANT_H
