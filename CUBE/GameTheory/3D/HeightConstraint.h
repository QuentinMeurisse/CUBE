#ifndef CUBE_HEIGHTCONSTRAINT_H
#define CUBE_HEIGHTCONSTRAINT_H

#include "../segment.h"
#include "group_of_cells3D.h"

#include <memory>

class HeightConstraint {
public:
    /**
     *
     * @param angle angle of the projection in degrees
     * @param projectionHeight
     */
    HeightConstraint(double angle, double projectionHeight);

    /**
     *
     * @param s
     * @param b1
     * @param b2
     * @return
     */
    [[nodiscard]] bool valid(Segment const& s,
                             const std::shared_ptr<GroupOfCells3D>& b1,
                             const std::shared_ptr<GroupOfCells3D>& b2) const;

    [[nodiscard]] bool valid(const std::shared_ptr<GroupOfCells3D>& b1,
                             const std::shared_ptr<GroupOfCells3D>& b2) const;

private:
    double angle; //angle of projection in radians
    double projection_height; // height we start the projection

};


#endif //CUBE_HEIGHTCONSTRAINT_H
