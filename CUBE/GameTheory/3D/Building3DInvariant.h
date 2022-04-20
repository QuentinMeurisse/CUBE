#ifndef CUBE_BUILDING3DINVARIANT_H
#define CUBE_BUILDING3DINVARIANT_H

#include "../Invariants/BuildingInvariant.h"

class Building3DInvariant : public BuildingInvariant{
public:
    Building3DInvariant(const std::shared_ptr<UrbanBlockGame> &g, double floor);

    std::shared_ptr<GroupOfCells>
    createGroupOfCells(const std::shared_ptr<Tiling> &tiling, unsigned int idnum, const std::string &c) override;

private:
    double floor_height;
};


#endif //CUBE_BUILDING3DINVARIANT_H
