#ifndef CUBE_BUILDINGINVARIANT_H
#define CUBE_BUILDINGINVARIANT_H


#include "CoModInvariant.h"
#include "../GroupOfCells.h"
#include "../UrbanBlockGame.h"

#include <map>
#include <memory>
#include <optional>

class BuildingInvariant: public CoModInvariant {
public:
    explicit BuildingInvariant(const std::shared_ptr<UrbanBlockGame> &g);
};


#endif //CUBE_BUILDINGINVARIANT_H
