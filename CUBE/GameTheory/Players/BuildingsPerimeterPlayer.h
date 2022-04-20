#ifndef CUBE_BUILDINGSPERIMETERPLAYER_H
#define CUBE_BUILDINGSPERIMETERPLAYER_H

#include "Player.h"

#include <random>
#include <functional>


class BuildingsPerimeterPlayer : public Player {

public:
    BuildingsPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum, unsigned int const &rb,
                             unsigned int const &rg);

    BuildingsPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum, unsigned int const &rb,
                             unsigned int const &rg, std::function<bool (double, double)> f);

    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    unsigned int minum;
    unsigned int maxnum;
    unsigned int rb, rg;

};


#endif //CUBE_BUILDINGSPERIMETERPLAYER_H
