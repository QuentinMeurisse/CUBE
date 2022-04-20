#ifndef CUBE_RADIUSCONSTRAINT_H
#define CUBE_RADIUSCONSTRAINT_H

#include <memory>
#include "Strategies/ChangeColorStrat.h"
#include "UrbanBlockGame.h"

class RadiusConstraint {
public:
    /**
     *
     * @param radius if radius equals 0, ie means that the constraint is disabled.
     * So the validation method returns always true
     */
    explicit RadiusConstraint(unsigned int radius);

    bool validChangeColor(const std::shared_ptr<UrbanBlockGame> &game, const std::shared_ptr<ChangeColorStrat>& strat);

private:
    unsigned int radius;

};


#endif //CUBE_RADIUSCONSTRAINT_H
