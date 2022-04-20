#include "RegularTiling.h"

using Mesh =  OpenMesh::PolyMesh_ArrayKernelT<>;

RegularTiling::RegularTiling(int width, int height, double const &cellsArea, unsigned int const &n)
        : cellsArea(cellsArea), n(n),
          Tiling(width, height) {}

double RegularTiling::getCellsArea() const {
    return cellsArea;
}

double RegularTiling::totalArea() const {
    return numCells() * cellsArea;
}

Mesh::Point RegularTiling::centroidOf(const unsigned int &k) {
    Mesh::Point center = {0., 0., 0.};
    Mesh::FHandle f = (*this)[k];

    for(Mesh::FVIter v = cells->fv_iter(f); v.is_valid(); ++v) {
        Mesh::Point p =  cells->point(*v);
        center += p;
    }

    return (1./n) * center;
}
