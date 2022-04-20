#ifndef CUBE_TILING_H
#define CUBE_TILING_H

#include "Segment.h"

#include <string>
#include <memory>
#include <stack>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>


class Tiling {
public:
    Tiling(int width, int height);

    virtual ~Tiling();

    void init(); // This method generate a tiling. It calls the generate() virtual method

    [[nodiscard]] std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>> getCells() const;

    [[nodiscard]] std::string getColor(const unsigned int &k) const;

    void setColor(const unsigned int &k, std::string const& color);

    [[nodiscard]] unsigned int getComponent(const unsigned int &k) const;

    void setComponent(const unsigned int &k, unsigned int const& num);

    double cellsPerimeter(const unsigned int &k);

    double sideLength(const unsigned int &fid1, const unsigned int &fid2);

    unsigned int closerComponent(const unsigned int &k, std::string const& color);

    unsigned int nextId();

    void releaseId(unsigned int const& num);

    /**
     *
     * @param fid
     * @param color
     * @param excluding
     * @return test all the adjacent faces of fid are colored or not or if fid is on the border of the mesh.
     * Used to determine if fid is inside or outside a building.
     */
    bool allAreFilled(unsigned int const& fid, std::string const& color, unsigned int excluding=0);

    [[nodiscard]] int numCells() const;

    OpenMesh::FaceHandle operator[](const unsigned int &k) const;

    [[nodiscard]] virtual double getCellsArea() const = 0;

    [[nodiscard]] virtual double totalArea() const  = 0;

    [[nodiscard]] virtual OpenMesh::PolyMesh_ArrayKernelT<>::Point centroidOf(const unsigned int &k) = 0;

    [[nodiscard]] virtual int getWidth() const;

    [[nodiscard]] virtual int getHeight() const;

    std::vector<std::pair<double, double>> vertexOf(const unsigned int &k);

    std::set<Segment> edgesOf(const unsigned int& k);

    [[nodiscard]] std::pair<uint, uint> adjacentFaces(Segment const& s) const;

protected:
    std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>> cells;
    unsigned int ids;
    std::stack<unsigned int > reusable;
    int width;
    int height;

    /**
     *
     * @param p
     * @param fid
     * @return If the point p belongs to the face fid, return the id of the vertex_handle. Else return -1
     */
    int checkIfPointBelongToFace(OpenMesh::PolyMesh_ArrayKernelT<>::Point p, int fid);

    virtual void generate() = 0;

};


#endif //CUBE_TILING_H
