#ifndef CUBE_SQUARETILING_H
#define CUBE_SQUARETILING_H


#include "RegularTiling.h"

class SquareTiling: public RegularTiling {
public:
    SquareTiling(int width, int height, const double &cellsArea);

protected:
    void generate() override;

};


#endif //CUBE_SQUARETILING_H
