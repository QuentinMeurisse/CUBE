#ifndef CUBE_NUMBUILDING3DPLAYER_H
#define CUBE_NUMBUILDING3DPLAYER_H

#include "Player.h"
#include "../TwoRadiusConstraint.h"
#include "HeightConstraint.h"



class NumBuilding3DPlayer : public Player{
public:
    NumBuilding3DPlayer(const unsigned int &minnum,
                        const unsigned int &maxnum,
                        unsigned int const &rb,
                        unsigned int const &rg,
                        double const& angle,
                        double const& height_proj);

    NumBuilding3DPlayer(const unsigned int &minnum,
                        const unsigned int &maxnum,
                        unsigned int const &rb,
                        unsigned int const &rg,
                        double const& angle,
                        double const& height_proj,
                        std::function<bool (double, double)> f);

public:
    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    unsigned int minnum ;
    unsigned int maxnum;
    TwoRadiusConstraint radius_constraint = TwoRadiusConstraint(0, 0);
    HeightConstraint height_constraint = HeightConstraint(0, 0);

};


#endif //CUBE_NUMBUILDING3DPLAYER_H
