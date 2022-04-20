#ifndef CUBE_COMOD3DINVARIANT_H
#define CUBE_COMOD3DINVARIANT_H

#include "../Invariants/CoModInvariant.h"

class CoMod3DInvariant : public CoModInvariant{
public:
    CoMod3DInvariant(const std::shared_ptr<UrbanBlockGame> &g, std::string color);

    std::shared_ptr<GroupOfCells>
    createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum, const std::string &c) override;
};


#endif //CUBE_COMOD3DINVARIANT_H
