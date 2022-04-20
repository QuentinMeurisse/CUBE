#ifndef CUBE_GROUP_OF_CELLS3D_H
#define CUBE_GROUP_OF_CELLS3D_H

#include "../GroupOfCells.h"
#include "../SquareTiling.h"
#include "../segment.h"
#include "segmenttree.h"

#include <string>

class GroupOfCells3D : public GroupOfCells{

public:
    GroupOfCells3D(const std::shared_ptr<SquareTiling> &tiling, unsigned int idnum, const std::string &color);

    void addCell(const unsigned int &fid) override;

    void removeCell(const unsigned int &fid) override;

    [[nodiscard]] const std::shared_ptr<SegmentTree<SegmentYorder>> &getHorizontalEdges() const;

    [[nodiscard]] const std::shared_ptr<SegmentTree<>> &getVerticalEdges() const;

    void increaseHeight(double x);

    void decreaseHeight(double x);

    double getHeight() const;

    void write(const std::string& path, bool overwrite = false);

    static double distance(const GroupOfCells3D& goc1, const GroupOfCells3D& goc2, bool vertical);

    static double distance(const Segment& seg, const GroupOfCells3D& goc, bool vertical);


private:
    std::shared_ptr<SegmentTree<SegmentYorder>> horizontalEdges;
    std::shared_ptr<SegmentTree<>> verticalEdges;

    double height = 0;

    void updateInsertEdges(const unsigned int &fid);

    void updateRemoveEdges(const unsigned int &fid);

    void extendSegment(Segment seg, const std::shared_ptr<TreeNode<Segment>>& n);

    void mergeEdges(Segment const& bridge);

    void cutEdge(Segment const& cut, const std::shared_ptr<TreeNode<Segment>>& n);

    Segment findOrthogonalSegment(Segment const& s,
                                  std::pair<double, double> const& extremity,
                                  std::set<Segment> const& edges);
};


#endif //CUBE_GROUP_OF_CELLS3D_H
