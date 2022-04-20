#ifndef CUBE_REGULARTILING_H
#define CUBE_REGULARTILING_H


#include "Tiling.h"

class RegularTiling: public Tiling {

public:
    RegularTiling(int width, int height, double const &cellsArea, unsigned int const &n);

    [[nodiscard]] double getCellsArea() const override;

    [[nodiscard]] double totalArea() const override ;

    OpenMesh::PolyMesh_ArrayKernelT<>::Point centroidOf(const unsigned int &k) override;


protected:
    double cellsArea;
    unsigned int n;

};


#endif //CUBE_REGULARTILING_H
