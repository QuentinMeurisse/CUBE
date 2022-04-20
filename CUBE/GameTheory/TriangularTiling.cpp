#include "TriangularTiling.h"
#include <cmath>
#include <functional>

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using namespace std;

TriangularTiling::TriangularTiling(int width, int height, const double &cellsArea) : RegularTiling(width, height, cellsArea, 3) {}

void TriangularTiling::generate() {
    function <vector<Mesh::Point> (int, double, double, double )> nRoots = [&](int n, double cx, double cy, double angle){
        vector<Mesh::Point> to_return;
        for (int k = 0; k < n; ++k) {
            double x = cos((2 * k * M_PI) / n) ;
            double y = sin((2 * k * M_PI) / n);
            double x2 = x * cos(angle) - y * sin(angle);
            double y2 = x * sin(angle) + y * cos(angle);
            to_return.emplace_back(x2 + cx, y2 + cy, 0);
        }
        return to_return;
    };
    double const a = cos(M_PI / 3); //apothem of the triangle
    double lastY = 0;
    for (int i = 0; i < height; ++i){
        double cx = 0;
        double cy = i == 0 ? 0 : i % 2 == 0 ? lastY + 2 * a : lastY + 2;
        lastY = cy;
        for (int j = 0; j < width; ++j) {
            double theta;
            if (i % 2 == 0)
                theta = j%2 == 0 ? M_PI / 2 : -1 * M_PI / 2;
            else
                theta = j%2 == 0 ? -1 * M_PI / 2 : M_PI / 2;

            vector<Mesh::Point> points = nRoots(3, cx, cy, theta);
            vector<Mesh::VertexHandle> vertices;
            int num_faces = int(cells->n_faces());
            for (auto const& p : points){
                int already_known = -1;
                if (j != 0)
                    already_known = checkIfPointBelongToFace(p, num_faces - 1);
                if (i != 0 and already_known == -1) //If we found the point we don't need to search it
                    already_known = checkIfPointBelongToFace(p, num_faces - width);

                if (already_known != -1)
                    vertices.push_back(cells->vertex_handle(already_known));
                else
                    vertices.push_back(cells->add_vertex(p)); //create a new point
            }
            cells->add_face(vertices);
            if (i % 2 == 0){
                cx += cos(M_PI / 6);
                cy = j%2 == 0 ? cy + 2 * a * sin(M_PI / 6) : cy + 2 * a * sin(-1 * M_PI / 6);
            } else{
                cx += cos(M_PI / 6);
                cy = j%2 == 0 ? cy + 2 * a * sin(-1 * M_PI / 6) : cy + 2 * a * sin(M_PI / 6);
            }
        }
    }
}
