#ifndef CUBE_INFLATE3DSTRAT_H
#define CUBE_INFLATE3DSTRAT_H

#include "Strategy.h"
#include "../TwoRadiusConstraint.h"
#include "HeightConstraint.h"

#include <deque>
#include <memory>

class Inflate3DStrat : public Strategy {
public:
    Inflate3DStrat(int const &idnum,
                   std::string color,
                   unsigned int const &rb,
                   unsigned int const &rg,
                   double const& angle,
                   double const& project_height);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    bool equal(const Strategy &other) const override;

protected:
    void print(std::ostream &os) const override;


private:
    int idnum;
    std::deque<std::shared_ptr<Strategy>> q;
    TwoRadiusConstraint radius_constraint  = TwoRadiusConstraint(0, 0);
    std::string color;
    HeightConstraint height_constraint = HeightConstraint(0, 0);

};


#endif //CUBE_INFLATE3DSTRAT_H
