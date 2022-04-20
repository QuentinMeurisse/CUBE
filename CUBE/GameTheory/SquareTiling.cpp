#include "SquareTiling.h"

#include <cmath>
#include <iostream>

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using namespace std;

SquareTiling::SquareTiling(int width, int height, const double &cellsArea) : RegularTiling(width, height, cellsArea, 4) {}

void SquareTiling::generate() {
    double sideLength = sqrt(cellsArea);
    for (int i = 0; i < height + 1; ++i) { // If we want n faces on the height, we need n+1 vertices
        for (int j = 0; j < width + 1; ++j) {
            cells->add_vertex(Mesh::Point(j * sideLength, i * sideLength, 0));
            if (i != 0 && j != 0){
                unsigned long k = cells->n_vertices() - 1;
                cells->add_face(
                        cells->vertex_handle(k),
                        cells->vertex_handle(k - 1),
                        cells->vertex_handle(k - 1 - (width + 1)),
                        cells->vertex_handle(k- (width + 1)));
            }
        }
    }
}
