#include "GroupOfCells.h"

#include <utility>
#include <iostream>
#include "GameTheoryUtility.h"

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using MeshPtr = std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>>;
using namespace std;

GroupOfCells::GroupOfCells(shared_ptr<Tiling> tiling, unsigned int idnum, string color)
        : tiling(move(tiling)), idnum(idnum), color(move(color)) {
    area = 0.;
    perimeter = 0.;
    center = {0., 0., 0.};
}

double GroupOfCells::getArea() const {
    return area;
}

double GroupOfCells::getPerimeter() const {
    return perimeter;
}

unsigned int GroupOfCells::getId() const {
    return idnum;
}

const string &GroupOfCells::getColor() const {
    return color;
}

void GroupOfCells::addCell(const unsigned int &fid) {
    MeshPtr mesh = tiling->getCells();
    cells.insert(fid);
    if (tiling->allAreFilled(fid, color))
        inner.insert(fid);
    else
        border.insert(fid);
    double fidArea = tiling->getCellsArea();
    area += fidArea;
    center += fidArea * tiling->centroidOf(fid);

    for (Mesh::FaceFaceIter f = mesh->ff_iter(mesh->face_handle(fid)); f.is_valid(); ++f){
    // For all adjacent cells to fid
        if (tiling->getColor(f->idx()) == color and tiling->getComponent(f->idx()) == idnum){
        // If our neighbor is colored, the perimeter decrease
            perimeter -= tiling->sideLength(fid, f->idx());
            if (tiling->allAreFilled(f->idx(), color, fid)){
                border.erase(f->idx());
                inner.insert(f->idx());
            }
        }
        else // The perimeter increase
            perimeter += tiling->sideLength(fid, f->idx());
    }
    if(mesh->is_boundary((*tiling)[fid])){
    // If fid is a boundary cell, its sides without neighbors were not visited by the  previous loop
        for (Mesh::FEIter fe = mesh->fe_iter((*tiling)[fid]); fe.is_valid(); ++fe){
        // For all the sides of the cell
            Mesh ::EHandle eh = mesh->edge_handle(fe->idx());
            if (mesh->is_boundary(eh)){
            // If the side is on the boundary, we add its length to the perimeter.
            // If not we have already its length during the previous loop
                Mesh::HHandle heh = mesh->halfedge_handle(eh, 0);
                Mesh::VHandle vh1 = mesh->from_vertex_handle(heh);
                Mesh::VHandle vh2 = mesh->to_vertex_handle(heh);
                Mesh::Point p1 = mesh->point(vh1);
                Mesh::Point p2 = mesh->point(vh2);
                perimeter += GameTheoryUtility::norm(p1 - p2);
            }
        }
    }
}

void GroupOfCells::removeCell(const unsigned int &fid) {
    if (contains(fid)){
        cells.erase(fid);
        inner.erase(fid);
        border.erase(fid);

        MeshPtr mesh = tiling->getCells();

        double fidArea = tiling->getCellsArea();
        area -= fidArea;
        center -= fidArea * tiling->centroidOf(fid);

        for (Mesh::FaceFaceIter f = mesh->ff_iter(mesh->face_handle(fid)); f.is_valid(); ++f){
            if (tiling->getColor(f->idx()) == color and tiling->getComponent(f->idx()) == idnum){
                perimeter += tiling->sideLength(fid, f->idx());
                if (tiling->allAreFilled(f->idx(), color, fid)){
                    //If a neighbor of fid has a white neighbor other than fid it is already in border
                    inner.erase(f->idx());
                    border.insert(f->idx());
                }
            }
            else
                perimeter -= tiling->sideLength(fid, f->idx());
        }
        if(mesh->is_boundary((*tiling)[fid])){
            for (Mesh::FEIter fe = mesh->fe_iter((*tiling)[fid]); fe.is_valid(); ++fe){
                Mesh ::EHandle eh = mesh->edge_handle(fe->idx());
                if (mesh->is_boundary(eh)){
                    Mesh::HHandle heh = mesh->halfedge_handle(eh, 0);
                    Mesh::VertexHandle vh1 = mesh->from_vertex_handle(heh);
                    Mesh::VertexHandle vh2 = mesh->to_vertex_handle(heh);
                    Mesh::Point p1 = mesh->point(vh1);
                    Mesh::Point p2 = mesh->point(vh2);
                    perimeter -= GameTheoryUtility::norm(p1 - p2);
                }
            }
        }
    }
}

bool GroupOfCells::contains(const unsigned int &fid) {
    return cells.count(fid);
}

ostream &operator<<(ostream &os, const GroupOfCells &groupOfCells) {
    os << "GroupOfCells " << groupOfCells.idnum << " border: { ";
    for (auto x : groupOfCells.border){
        os << x << " ";
    }
    os << "}";
    os << "\tinner: { ";
    for (auto x : groupOfCells.inner){
        os << x << " ";
    }
    os << "}";
    return os;
}

const set<unsigned int> &GroupOfCells::getCells() const {
    return cells;
}

const set<unsigned int> &GroupOfCells::getInner() const {
    return inner;
}

const set<unsigned int> &GroupOfCells::getBorder() const {
    return border;
}

Mesh::Point GroupOfCells::centroid() {
    return (1. / area) * center;
}

void GroupOfCells::updateCellsComponent() {
    for (unsigned int k : cells)
        tiling->setComponent(k, idnum);
}

GroupOfCells::~GroupOfCells() = default;
