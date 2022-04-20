#ifndef CUBE_POROSITYPLAYER_H
#define CUBE_POROSITYPLAYER_H

#include "Player.h"
#include "../TwoRadiusConstraint.h"

#include <functional>

class PorosityPlayer: public Player {
public:
    PorosityPlayer(unsigned int rb, unsigned int rg);

    PorosityPlayer(unsigned int rb, unsigned int rg, std::function<bool (double, double)> f);


    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    TwoRadiusConstraint constraint = TwoRadiusConstraint(0, 0);
};


#endif //CUBE_POROSITYPLAYER_H
