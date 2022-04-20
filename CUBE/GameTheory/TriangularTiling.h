#ifndef CUBE_TRIANGULARTILING_H
#define CUBE_TRIANGULARTILING_H

#include "RegularTiling.h"
class TriangularTiling : public RegularTiling{
public:
    TriangularTiling (int width, int height, const double &cellsArea);

protected:
    void generate() override;

};


#endif //CUBE_TRIANGULARTILING_H
