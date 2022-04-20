#ifndef CUBE_TWORADIUSCONSTRAINT_H
#define CUBE_TWORADIUSCONSTRAINT_H

#include "UrbanBlockGame.h"
#include "Strategies/ChangeColorStrat.h"

#include <memory>

class TwoRadiusConstraint {
public:
    TwoRadiusConstraint(unsigned int buildRadius, unsigned int greenRadius);

    bool validChangeColor(const std::shared_ptr<UrbanBlockGame> &game, const std::shared_ptr<ChangeColorStrat>& strat);

private:
    unsigned int buildRadius, greenRadius;

    [[nodiscard]] bool validChangeColorOnGreenSpace(const std::shared_ptr<UrbanBlockGame>& game,
                                      const std::shared_ptr<ChangeColorStrat>& strat) const;

    [[nodiscard]] bool validChangeColorOnBuilding(const std::shared_ptr<UrbanBlockGame>& game,
                                    const std::shared_ptr<ChangeColorStrat>& strat) const;

};


#endif //CUBE_TWORADIUSCONSTRAINT_H
