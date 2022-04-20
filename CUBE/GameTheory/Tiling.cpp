#include "Tiling.h"
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <iostream>
#include "GameTheoryUtility.h"

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using MeshPtr = std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>>;
using namespace std;

Tiling::Tiling(int width, int height): width(width), height(height) {
    ids = 0;
}

Tiling::~Tiling() = default;



void Tiling::init() {
    cells = make_shared<Mesh >();
    generate();

    auto rgb = OpenMesh::getOrMakeProperty<OpenMesh::FaceHandle, string>(*cells, "rgb");
    auto component = OpenMesh::getOrMakeProperty<OpenMesh::FaceHandle, unsigned int>(*cells, "component");
    for (Mesh::FaceHandle fh : cells->faces()){
        rgb[fh] = "#ffffff";
        component[fh] = 0;
    }
}

std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>> Tiling::getCells() const {
    return cells;
}

string Tiling::getColor(const unsigned int &k) const{
    OpenMesh::FaceHandle f = (*this)[k];
    auto rgb = OpenMesh::getProperty<OpenMesh::FaceHandle, string>(*cells, "rgb");
    return rgb[f];
}

void Tiling::setColor(const unsigned int &k, string const& color) {
    OpenMesh::FaceHandle f = (*this)[k];
    auto rgb = OpenMesh::getProperty<OpenMesh::FaceHandle, string>(*cells, "rgb");
    rgb[f] = color;
}

unsigned int Tiling::getComponent(const unsigned int &k) const{
    OpenMesh::FaceHandle f = (*this)[k];
    auto component = OpenMesh::getProperty<OpenMesh::FaceHandle, unsigned int >(*cells, "component");
    return component[f];
}

void Tiling::setComponent(const unsigned int &k, unsigned int const& num) {
    OpenMesh::FaceHandle f = (*this)[k];
    auto component = OpenMesh::getProperty<OpenMesh::FaceHandle, unsigned int >(*cells, "component");
    component[f] = num;
}

double Tiling::cellsPerimeter(const unsigned int &k) {
    OpenMesh::FaceHandle f = (*this)[k];
    vector<Mesh::Point> points;

    for(Mesh::FaceVertexIter fv = cells->fv_iter(f); fv.is_valid(); ++fv){
        points.push_back(cells->point(*fv));
    }
    double perimeter = 0.;
    for (unsigned long i = 0; i < points.size(); ++i) {
        Mesh::Point x = points[i];
        Mesh::Point y = points[(i + 1) % points.size()];
        perimeter += GameTheoryUtility::norm(x - y);
    }
    return perimeter;
}

double Tiling::sideLength(const unsigned int &fid1, const unsigned int &fid2) {
    Mesh::FaceHandle f1 = (*this)[fid1];
    Mesh::FaceHandle f2 = (*this)[fid2];
    vector<Mesh::Point> pointsf1;
    vector<Mesh::Point> pointsf2;
    for (Mesh::FaceVertexIter fv = cells->fv_iter(f1); fv.is_valid(); ++fv){
        pointsf1.push_back(cells->point(*fv));
    }
    for (Mesh::FaceVertexIter fv = cells->fv_iter(f2); fv.is_valid(); ++fv){
        pointsf2.push_back(cells->point(*fv));
    }

    vector<Mesh::Point> intersection;
    sort(pointsf1.begin(), pointsf1.end());
    sort(pointsf2.begin(), pointsf2.end());
    set_intersection(pointsf1.begin(), pointsf1.end(), pointsf2.begin(), pointsf2.end(), back_inserter(intersection));

    if (intersection.size() != 2){
        throw invalid_argument("These two faces are not adjacent");
    }
    Mesh::Point p1 = intersection.back();
    intersection.pop_back();
    Mesh::Point p2 = intersection.back();
    intersection.pop_back();

    return GameTheoryUtility::norm(p1 - p2);
}

unsigned int Tiling::closerComponent(const unsigned int &k, std::string const &color) {
    deque<unsigned int> close = GameTheoryUtility::faceBall(cells, k, 1);
    for(unsigned int i : close){
        if (getColor(i) == color){
            return getComponent(i);
        }
    }
    return 0;
}

unsigned int Tiling::nextId() {
    if(!reusable.empty()){
        unsigned x = reusable.top();
        reusable.pop();
        return x;
    }
    else{
        return ++ids;
    }

}

void Tiling::releaseId(unsigned int const &num) {
    reusable.push(num);
}

/**
 * Use to test it the cell fid is in the border or the inner of a component
 * @param fid
 * @param color
 * @param excluding
 * @return
 */
bool Tiling::allAreFilled(unsigned int const &fid, std::string const &color, unsigned int excluding) {
    if (cells->is_boundary((*this)[fid])){
        return false;
    }
    else{
        for(Mesh::FaceFaceIter f = cells->ff_iter((*this)[fid]); f.is_valid(); ++f){
            if(getColor(f->idx()) != color && f->idx() != excluding){
                return false;
            }
        }
        return true;
    }
}

int Tiling::numCells() const{
    return cells->n_faces();
}

OpenMesh::FaceHandle Tiling::operator[](const unsigned int &k) const {
    return cells->face_handle(k);
}

int Tiling::getWidth() const {
    return width;
}

int Tiling::getHeight() const {
    return height;
}

vector<pair<double, double>> Tiling::vertexOf(const unsigned int &k) {
    vector<pair<double, double>> to_return;
    auto c = (*this)[k];
    for (Mesh::FVIter fv_iter = cells->fv_iter(c); fv_iter.is_valid(); fv_iter++){
        auto p = cells->point(*fv_iter);
        pair<double, double> xy(p[0], p[1]);
        to_return.push_back(xy);
    }
    return to_return;
}

std::set<Segment> Tiling::edgesOf(const unsigned int &k) {
    set<Segment> to_return;
    auto c = (*this)[k];
    for (Mesh::FHIter fh_iter = cells->fh_iter(c); fh_iter.is_valid(); fh_iter++){
        Mesh::Point from = cells->point(cells->from_vertex_handle(*fh_iter));
        Mesh::Point to = cells->point(cells->to_vertex_handle(*fh_iter));
        Segment s(from[0], from[1], to[0], to[1]);
        to_return.insert(s);
    }
    return to_return;
}

std::pair<uint, uint> Tiling::adjacentFaces(const Segment &s) const {
    if (s.is_vertical()){
        uint a = this->width * s.getAlpha().second + s.getAlpha().first - 1;
        uint b = this->width * s.getAlpha().second + s.getAlpha().first;
        return std::pair<uint, uint>(a, b);
    } else if (s.is_horizontal()){
        uint a = this->width * (s.getAlpha().second - 1) + s.getAlpha().first;
        uint b = this->width * s.getAlpha().second + s.getAlpha().first;
        return std::pair<uint, uint>(a, b);
    }
    else
        return std::pair<uint, uint>();
}

int Tiling::checkIfPointBelongToFace(OpenMesh::PolyMesh_ArrayKernelT<>::Point p, int fid) {
    auto f = (*this)[fid];
    for (Mesh::FVIter fviter = cells->fv_iter(f); fviter.is_valid(); ++fviter){
        Mesh::Point q = cells->point(*fviter);
        if (abs(p[0] - q[0]) < 1e-5 and abs(p[1] - q[1]) < 1e-5)
            return fviter->idx();
    }
    return -1;
}
