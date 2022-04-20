#include "GroupOfCells3D.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_set>
#include <boost/functional/hash.hpp>

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using MeshPtr = std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>>;
using namespace std;

GroupOfCells3D::GroupOfCells3D(const std::shared_ptr<SquareTiling> &tiling, unsigned int idnum, const std::string &color)
        : GroupOfCells(tiling, idnum, color) {
    horizontalEdges = make_shared<SegmentTree<SegmentYorder>>();
    verticalEdges = make_shared<SegmentTree<>>();
}

void GroupOfCells3D::addCell(const unsigned int &fid) {
    GroupOfCells::addCell(fid);

    /*cout << "before insert" << endl;
    cout << "horizontal " << horizontalEdges->size() << endl;
    horizontalEdges->print();
    cout << endl;
    cout << "vertical " << verticalEdges->size() << endl;
    verticalEdges->print();
    cout << endl;*/

    updateInsertEdges(fid);

    /*cout << "after insert" << endl;
    cout << "horizontal " << horizontalEdges->size() << endl;
    horizontalEdges->print();
    cout << endl;
    cout << "vertical " << verticalEdges->size() << endl;
    verticalEdges->print();
    cout << endl;*/
}

void GroupOfCells3D::removeCell(const unsigned int &fid) {
    GroupOfCells::removeCell(fid);

    /*cout << "before remove" << endl;
    cout << "horizontal " << horizontalEdges->size() << endl;
    horizontalEdges->print();
    cout << endl;
    cout << "vertical" << verticalEdges->size() << endl;
    verticalEdges->print();
    cout << endl;*/

    updateRemoveEdges(fid);

    /*cout << "after remove" << endl;
    cout << "horizontal " << horizontalEdges->size() << endl;
    horizontalEdges->print();
    cout << endl;
    cout << "vertical" << verticalEdges->size() << endl;
    verticalEdges->print();
    cout << endl;*/
}

const shared_ptr<SegmentTree<SegmentYorder>> &GroupOfCells3D::getHorizontalEdges() const {
    return horizontalEdges;
}

const shared_ptr<SegmentTree<>> &GroupOfCells3D::getVerticalEdges() const {
    return verticalEdges;
}

void GroupOfCells3D::increaseHeight(double x) {
    height += x;
}

void GroupOfCells3D::decreaseHeight(double x) {
    height -= x;
}

double GroupOfCells3D::getHeight() const {
    return height;
}

void GroupOfCells3D::write(const std::string& path, bool overwrite) {

    nlohmann::json j;
    if (not overwrite){
        ifstream reader(path);
        if (reader.is_open()) {
            reader >> j;
        }
        reader.close();
    }
    string key = to_string(idnum);
    j[key]["height"] = height;
    vector<pair<double, double>> v;
    Segment current = horizontalEdges->getRoot()->data;
    bool search_alpha = false;
    do {
        v.push_back(current.getAlpha());
        v.push_back(current.getBeta());
        Segment extremity = search_alpha ? Segment(current.getAlpha(), current. getBeta())
                                         : Segment(current.getBeta(), current.getBeta());

        auto next_node = current.is_horizontal() ? verticalEdges->search_segment_containing(extremity)
                                                 : horizontalEdges->search_segment_containing(extremity);
        search_alpha = extremity.getAlpha() == next_node->data.getBeta();
        current = next_node->data;
    } while (current != horizontalEdges->getRoot()->data);
    unordered_set<pair<double, double>, boost::hash<pair<double, double>>> u_set;
    auto end = remove_if(v.begin(), v.end(),
                         [&](pair<double, double> const& p){
                             return not u_set.insert(p).second;
                         });
    v.erase(end, v.end());
    j[key]["base"] = v;
    j[key]["color"] = color;
    ofstream writer;
    writer.open(path, ios::out);
    writer << j.dump(4);
    writer.close();
}


double GroupOfCells3D::distance(const GroupOfCells3D& goc1, const GroupOfCells3D& goc2, bool vertical) {
    vector<Segment> fronts1 = vertical ? goc1.verticalEdges->values() : goc1.horizontalEdges->values();
    vector<Segment> fronts2 = vertical ? goc2.verticalEdges->values() : goc2.horizontalEdges->values();
    double dist = numeric_limits<double>::infinity();
    for (Segment const& s1 : fronts1){
        for (Segment const& s2 : fronts2){
            double d = Segment::distance(s1, s2);
            if (d < dist)
                dist = d;
        }
    }
    return dist;
}

double GroupOfCells3D::distance(const Segment &seg, const GroupOfCells3D &goc, bool vertical) {
    vector<Segment> front = vertical ? goc.verticalEdges->values() : goc.getHorizontalEdges()->values();
    double dist = numeric_limits<double>::infinity();
    for (Segment const& s : front){
        double d = Segment::distance(seg, s);
        if (d < dist)
            dist = d;
    }
    return dist;
}

void GroupOfCells3D::updateInsertEdges(const unsigned int &fid) {
    set<Segment> edges_fid = tiling->edgesOf(fid);
    for (Segment const& e : edges_fid){
        auto e_node = e.is_vertical() ? verticalEdges->search_segment_containing(e)
                                      : horizontalEdges->search_segment_containing(e);
        if (e_node != nullptr){
            cutEdge(e, e_node);
        }
        else{
            /* The segment e isn't contained in any edges, but it can have an extremity in common with an edge.
             * If e don't have any extremity in common with an existing edge, the segment e is a new edge.
             * If e has a common extremity with another edge, we need to check if this edge has the
             * same orientation (north/south for a vertical edge, west/east for horizontal) as e.
             * If yes, e extend the edge (or perform a merge if e intersects two edges).
             * If not, e doesn't extend the edge and e is simply a new edge.
             * To check if the edge has the same orientation as e, we need first to know the segment in fid
             * orthogonal to e in the common extremity. Thanks this orthogonal segment we can have the id
             * of the neighbor of fid. If this neighbor has the same color as this->color, the edge has the same
             * orientation*/
            Segment alphaS(e.getAlpha(), e.getAlpha());
            Segment betaS(e.getBeta(), e.getBeta());
            auto alpha_node = e.is_vertical() ? verticalEdges->search_segment_containing(alphaS)
                                              : horizontalEdges->search_segment_containing(alphaS);

            auto beta_node = e.is_vertical() ? verticalEdges->search_segment_containing(betaS)
                                             : horizontalEdges->search_segment_containing(betaS);

            if (alpha_node == nullptr and beta_node == nullptr){
                e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
            } else if (alpha_node != nullptr and beta_node != nullptr){
                Segment ortho_alpha = findOrthogonalSegment(e, e.getAlpha(), edges_fid);
                Segment ortho_beta = findOrthogonalSegment(e, e.getBeta(), edges_fid);
                auto alpha_faces = tiling->adjacentFaces(ortho_alpha);
                auto beta_faces = tiling->adjacentFaces(ortho_beta);
                uint adj_alpha_face = alpha_faces.first == fid ? alpha_faces.second : alpha_faces.first;
                uint adj_beta_face = beta_faces.first == fid ? beta_faces.second : beta_faces.first;
                if (tiling->getColor(adj_alpha_face) == this->color
                    and tiling->getColor(adj_beta_face) == this->color){
                    mergeEdges(e);
                } else if(tiling->getColor(adj_alpha_face) == this->color){
                    extendSegment(e, alpha_node);
                } else if (tiling->getColor(adj_beta_face) == this->color){
                    extendSegment(e, beta_node);
                } else{
                    e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
                }
            } else {
                Segment ortho = alpha_node != nullptr ? findOrthogonalSegment(e, e.getAlpha(), edges_fid)
                                                      : findOrthogonalSegment(e, e.getBeta(), edges_fid);
                auto ortho_faces = tiling->adjacentFaces(ortho);
                uint ortho_face = ortho_faces.first == fid ? ortho_faces.second : ortho_faces.first;
                if (tiling->getColor(ortho_face) == this->color){
                    alpha_node != nullptr ? extendSegment(e, alpha_node) : extendSegment(e, beta_node);
                }
                else{
                    e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
                }
            }
        }
    }
}

void GroupOfCells3D::updateRemoveEdges(const unsigned int &fid) {
    set<Segment> edges_fid = tiling->edgesOf(fid);
    for (Segment const& e : edges_fid){
        auto e_node = e.is_vertical() ? verticalEdges->search_segment_containing(e)
                                      : horizontalEdges->search_segment_containing(e);
        if (e_node != nullptr){
            cutEdge(e, e_node);
        }
        else{
            /*Same as for the updateInsertEdges.
             * But to check the a edge the neighbor must have NOT the same color as this->color */
            Segment alphaS(e.getAlpha(), e.getAlpha());
            Segment betaS(e.getBeta(), e.getBeta());
            auto alpha_node = e.is_vertical() ? verticalEdges->search_segment_containing(alphaS)
                                              : horizontalEdges->search_segment_containing(alphaS);

            auto beta_node = e.is_vertical() ? verticalEdges->search_segment_containing(betaS)
                                             : horizontalEdges->search_segment_containing(betaS);

            if (alpha_node == nullptr and beta_node == nullptr){
                e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
            } else if (alpha_node != nullptr and beta_node != nullptr){
                Segment ortho_alpha = findOrthogonalSegment(e, e.getAlpha(), edges_fid);
                Segment ortho_beta = findOrthogonalSegment(e, e.getBeta(), edges_fid);
                auto alpha_faces = tiling->adjacentFaces(ortho_alpha);
                auto beta_faces = tiling->adjacentFaces(ortho_beta);
                uint adj_alpha_face = alpha_faces.first == fid ? alpha_faces.second : alpha_faces.first;
                uint adj_beta_face = beta_faces.first == fid ? beta_faces.second : beta_faces.first;
                if (tiling->getColor(adj_alpha_face) != this->color
                    and tiling->getColor(adj_beta_face) != this->color){
                    mergeEdges(e);
                } else if(tiling->getColor(adj_alpha_face) != this->color){
                    extendSegment(e, alpha_node);
                } else if (tiling->getColor(adj_beta_face) != this->color){
                    extendSegment(e, beta_node);
                } else{
                    e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
                }
            } else {
                Segment ortho = alpha_node != nullptr ? findOrthogonalSegment(e, e.getAlpha(), edges_fid)
                                                      : findOrthogonalSegment(e, e.getBeta(), edges_fid);
                auto ortho_faces = tiling->adjacentFaces(ortho);
                uint ortho_face = ortho_faces.first == fid ? ortho_faces.second : ortho_faces.first;
                if (tiling->getColor(ortho_face) != this->color){
                    alpha_node != nullptr ? extendSegment(e, alpha_node) : extendSegment(e, beta_node);
                }
                else{
                    e.is_vertical() ? verticalEdges->insert(e) : horizontalEdges->insert(e);
                }
            }
        }
    }
}

void GroupOfCells3D::extendSegment(Segment seg, const std::shared_ptr<TreeNode<Segment>>& n) {
    if (seg.getAlpha() == n->data.getBeta()){
        n->data.setBeta(seg.getBeta());
    }
    else{ // seg.getBeta() == n->data.getAlpha()
        Segment newS(seg.getAlpha(), n->data.getBeta());
        if (newS.is_vertical()){
            verticalEdges->remove(n->data);
            verticalEdges->insert(newS);
        }
        else{
            horizontalEdges->remove(n->data);
            horizontalEdges->insert(newS);
        }
    }

}

void GroupOfCells3D::mergeEdges(const Segment &bridge) {
    Segment alphaS(bridge.getAlpha(), bridge.getAlpha());
    Segment betaS(bridge.getBeta(), bridge.getBeta());
    if (bridge.is_vertical()){
        auto alpha_node = verticalEdges->search_segment_containing(alphaS);
        auto beta_node = verticalEdges->search_segment_containing(betaS);
        alpha_node->data.setBeta(beta_node->data.getBeta());
        verticalEdges->remove(beta_node->data);
    }
    else {
        auto alpha_node = horizontalEdges->search_segment_containing(alphaS);
        auto beta_node = horizontalEdges->search_segment_containing(betaS);
        alpha_node->data.setBeta(beta_node->data.getBeta());
        horizontalEdges->remove(beta_node->data);
    }
}

void GroupOfCells3D::cutEdge(const Segment &cut, const std::shared_ptr<TreeNode<Segment>>& n) {
    if (cut.getBeta() != n->data.getBeta()) {
        Segment new_edge(cut.getBeta(), n->data.getBeta());
        new_edge.is_vertical() ? verticalEdges->insert(new_edge) : horizontalEdges->insert(new_edge);
    }
    if (cut.getAlpha() != n->data.getAlpha()) {
        n->data.setBeta(cut.getAlpha());
    }
        /*The order of segment hold on the first extremity first.
         * Since we assume that the segments are parallel and have no common point,
         * modifying the second end of a segment does not change its position in the tree.*/
    else{
        cut.is_vertical() ? verticalEdges->remove(n->data) : horizontalEdges->remove(n->data);
    }
    // If cut == n->data, it will be remove
}

Segment GroupOfCells3D::findOrthogonalSegment(const Segment &s, const pair<double, double> &extremity,
                                              const set<Segment> &edges) {
    for (Segment const& e : edges){
        if (e.is_vertical() == s.is_horizontal() and (e.getAlpha() == extremity or e.getBeta() == extremity))
            return e;
    }
    return Segment();
}

