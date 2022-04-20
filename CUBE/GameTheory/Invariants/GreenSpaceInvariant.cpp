#include "GreenSpaceInvariant.h"

#include <memory>

using namespace std;

GreenSpaceInvariant::GreenSpaceInvariant(const shared_ptr<UrbanBlockGame> &g) : CoModInvariant(g, "#00f000") {}
