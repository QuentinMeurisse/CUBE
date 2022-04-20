#include "HexagonalTiling.h"
#include <cmath>
#include <functional>

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using namespace std;

HexagonalTiling::HexagonalTiling(int width, int height, const double &cellsArea) : RegularTiling(width, height, cellsArea, 6){}

void HexagonalTiling::generate() {
    function <vector<Mesh::Point> (int, double, double)> nRoots = [&](int n, double cx, double cy){
        vector<Mesh::Point> to_return;
        for (int k = 0; k < n; ++k) {
            double x = cos((2 * k * M_PI) / n) + cx;
            double y = sin((2 * k * M_PI) / n) + cy;
            to_return.emplace_back(x, y, 0);
        }
        return to_return;
    };
    double const h = cos(M_PI / 6); //height of the hexagon

    for (int i = 0; i < height; ++i) {
        double cx = 0.;
        double cy = i * 2 * h;
        for (int j = 0; j < width; ++j) {
            vector<Mesh::Point> points = nRoots(6, cx, cy);
            vector<Mesh::VertexHandle> vertices;
            int num_faces = int(cells->n_faces());
            for (auto const& p : points){
                int already_known = -1;
                if (j != 0)
                    already_known = checkIfPointBelongToFace(p, num_faces - 1);
                if (i != 0 and already_known == -1){ //If we found the point we don't need to search it
                    already_known = checkIfPointBelongToFace(p, num_faces - width);
                    if (j % 2 == 0 and j != width-1 and already_known == -1)
                        already_known = checkIfPointBelongToFace(p, num_faces - width + 1);
                }
                if (already_known != -1)
                    vertices.push_back(cells->vertex_handle(already_known));
                else
                    vertices.push_back(cells->add_vertex(p)); //create a new point
            }
            cells->add_face(vertices);
            cx += 2 * h * cos(M_PI / 6);
            cy = j%2 == 0 ? cy + 2 * h * sin(M_PI / 6) : cy - 2 * h * sin(M_PI / 6);
        }
    }
}


