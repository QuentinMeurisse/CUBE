#ifndef CUBE_NUMBUILDINGPLAYER_H
#define CUBE_NUMBUILDINGPLAYER_H

#include "../TwoRadiusConstraint.h"
#include "Player.h"

#include <random>
#include <functional>


class NumBuildingPlayer : public Player{
public:
    NumBuildingPlayer(const unsigned int &minnum, const unsigned int &maxnum, unsigned int const &rb,
                      unsigned int const &rg);

    NumBuildingPlayer(const unsigned int &minnum, const unsigned int &maxnum, unsigned int const &rb,
                      unsigned int const &rg, std::function<bool (double, double)> f);

    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    unsigned int minnum ;
    unsigned int maxnum;
    TwoRadiusConstraint constraint = TwoRadiusConstraint(0, 0);


};


#endif //CUBE_NUMBUILDINGPLAYER_H
