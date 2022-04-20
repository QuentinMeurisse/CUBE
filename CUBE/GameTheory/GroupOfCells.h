#ifndef CUBE_GROUPOFCELLS_H
#define CUBE_GROUPOFCELLS_H

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <memory>
#include <set>
#include <string>
#include <ostream>

#include "Tiling.h"

class GroupOfCells {
public:
    GroupOfCells(std::shared_ptr<Tiling> tiling, unsigned int idnum, std::string color);

    virtual ~GroupOfCells();

    [[nodiscard]] double getArea() const;

    [[nodiscard]] double getPerimeter() const;

    [[nodiscard]] unsigned int getId() const;

    [[nodiscard]] const std::string &getColor() const;

    [[nodiscard]] const std::set<unsigned int> &getCells() const;

    [[nodiscard]] const std::set<unsigned int> &getInner() const;

    [[nodiscard]] const std::set<unsigned int> &getBorder() const;

    virtual void addCell(const unsigned int &fid);

    virtual void removeCell(const unsigned int &fid);

    bool contains(const unsigned int &fid);

    OpenMesh::PolyMesh_ArrayKernelT<>::Point centroid(); //TODO: remove

    /**
     * Change the component value of all cell belonging to the GroupOfCells (use for merge an de-merge)
     */
    void updateCellsComponent();

    friend std::ostream &operator<<(std::ostream &os, const GroupOfCells &groupOfCells);

protected:
    std::shared_ptr<Tiling> tiling;
    unsigned int idnum;
    double area;
    double perimeter;
    std::set<unsigned int > cells;
    std::set<unsigned int> inner;
    std::set<unsigned int> border;
    /*
     * We need to access easily to the on the border of a component (for the InflateStrat for example).
     * However using the sets inner and border to iterate on the cells of the component can cause side effects.
     * Especially for ChangeBlockColorStrat. This strat lead us to move some cells from inner to border.
     */
    std::string color;
    OpenMesh::PolyMesh_ArrayKernelT<>::Point center;

};


#endif //CUBE_GROUPOFCELLS_H
