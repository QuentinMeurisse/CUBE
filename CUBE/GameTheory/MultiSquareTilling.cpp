#include "MultiSquareTilling.h"

#include <cmath>
using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using namespace std;

MultiSquareTilling::MultiSquareTilling(int sub_width,
                                       int sub_height,
                                       const double &cellsArea,
                                       int blocksWidth,
                                       int blocksHeight,
                                       int blocksDistance) : RegularTiling(sub_width, sub_height, cellsArea, 4),
                                                             blocks_width(blocksWidth),
                                                             blocks_height(blocksHeight),
                                                             blocks_distance(blocksDistance) {}

void MultiSquareTilling::generate() {
    double side_length = sqrt(cellsArea);
    double y = 0;
    for (int i = 0; i < blocks_height; ++i) {
        double x = 0;
        for (int j = 0; j < blocks_width; ++j) {
            generate(x, y);
            x += width * side_length + blocks_distance;
        }
        y += height * side_length + blocks_distance;
    }

}

void MultiSquareTilling::generate(double startx, double straty) {
    double sideLength = sqrt(cellsArea);
    for (int i = 0; i < height + 1; ++i) { // If we want n faces on the height, we need n+1 vertices
        for (int j = 0; j < width + 1; ++j) {
            cells->add_vertex(Mesh::Point(startx + j * sideLength, straty + i * sideLength, 0));
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

int MultiSquareTilling::getWidth() const {
    return width * blocks_width + (blocks_width - 1) * blocks_distance ;
}

int MultiSquareTilling::getHeight() const {
    return height * blocks_height + (blocks_height - 1 ) * blocks_distance;
}

