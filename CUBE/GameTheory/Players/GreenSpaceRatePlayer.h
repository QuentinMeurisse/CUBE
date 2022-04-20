#ifndef COMODGAMECPP_GREENSPACERATEPLAYER_H
#define COMODGAMECPP_GREENSPACERATEPLAYER_H

#endif //COMODGAMECPP_GREENSPACERATEPLAYER_H

#include "Player.h"

#include <random>
#include <functional>

class GreenSpaceRatePlayer : public Player{

public:
    GreenSpaceRatePlayer(double const &ratio, unsigned int const &rb, unsigned int const &rg);
    GreenSpaceRatePlayer(double const &ratio, unsigned int const &rb, unsigned int const &rg,
            std::function<bool (double, double)> f);


    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    double ratio;
    unsigned int rb, rg;
};