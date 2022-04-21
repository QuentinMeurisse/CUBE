#ifndef COMODGAMECPP_GREENSPACEINVARIANT_H
#define COMODGAMECPP_GREENSPACEINVARIANT_H

#include <map>
#include <memory>
#include "UbranBlockGameInvariant.h"
#include "../GroupOfCells.h"
#include "../UrbanBlockGame.h"

class GreenSpaceInvariant: public UbranBlockGameInvariant {
public:
    explicit GreenSpaceInvariant(const std::shared_ptr<UrbanBlockGame> &g);

};


#endif //COMODGAMECPP_GREENSPACEINVARIANT_H
