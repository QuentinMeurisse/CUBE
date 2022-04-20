#ifndef CUBE_HEXAGONALTILING_H
#define CUBE_HEXAGONALTILING_H

#include "RegularTiling.h"
class HexagonalTiling : public RegularTiling {
public:
    HexagonalTiling(int width, int height, const double &cellsArea);

protected:
    void generate() override;
};


#endif //CUBE_HEXAGONALTILING_H
