#include "BuildingInvariant.h"
#include "../Strategies/ChangeColorStrat.h"

#include <memory>
#include <iostream>

using namespace std;

BuildingInvariant::BuildingInvariant(const shared_ptr<UrbanBlockGame> &g) : UbranBlockGameInvariant(g, "#f00000") {}

