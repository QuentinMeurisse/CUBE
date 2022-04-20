#ifndef COMODGAMECPP_NUMGREENSPACESPLAYER_H
#define COMODGAMECPP_NUMGREENSPACESPLAYER_H


#include "../TwoRadiusConstraint.h"
#include "Player.h"
#include <random>
#include <functional>

class NumGreenSpacesPlayer : public Player {
public:
    NumGreenSpacesPlayer(unsigned int const &minnum, unsigned int const &maxnum, unsigned int const &rb,
                         unsigned int const &rg);

    NumGreenSpacesPlayer(unsigned int const &minnum, unsigned int const &maxnum, unsigned int const &rb,
                         unsigned int const &rg, std::function<bool (double, double)> f);

    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    unsigned int minnum, maxnum;
    TwoRadiusConstraint constraint = TwoRadiusConstraint(0, 0);

};


#endif //COMODGAMECPP_NUMGREENSPACESPLAYER_H
