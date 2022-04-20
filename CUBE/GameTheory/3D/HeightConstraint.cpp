#include "HeightConstraint.h"

#include "cmath"

using namespace std;

HeightConstraint::HeightConstraint(double angle, double projectionHeight) : projection_height(projectionHeight) {
    double const pi = acos(-1);
    this->angle = (pi / 180) * angle;
}

bool HeightConstraint::valid(const Segment & s,
                             const std::shared_ptr<GroupOfCells3D>& b1,
                             const std::shared_ptr<GroupOfCells3D>& b2) const {
    double d = GroupOfCells3D::distance(s, *b2, s.is_vertical());
    double max_height = tan(angle) * d + projection_height;
    return b2->getHeight() <= max_height + 1e-10 and b1->getHeight() <= max_height + 1e-10;
}

bool HeightConstraint::valid(const shared_ptr<GroupOfCells3D> &b1, const shared_ptr<GroupOfCells3D> &b2) const {
    double horizontal_dist = GroupOfCells3D::distance(*b1, *b2, false);
    double vertical_dist = GroupOfCells3D::distance(*b1, *b2, true);
    double max_horizontal_height = tan(angle) * horizontal_dist + projection_height;
    double max_vertical_height = tan(angle) * vertical_dist + projection_height;
    bool is_b1_valid = b1->getHeight() <= max_horizontal_height + 1e-10 and b1->getHeight() <= max_vertical_height;
    bool is_b2_valid  = b2->getHeight() <= max_horizontal_height + 1e-10 and b2->getHeight() <= max_vertical_height;
    return is_b1_valid and is_b2_valid;
}


