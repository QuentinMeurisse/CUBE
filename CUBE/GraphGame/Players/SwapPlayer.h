#ifndef CUBE_SWAPPLAYER_H
#define CUBE_SWAPPLAYER_H

#include "Player.h"

#include <string>

class SwapPlayer: public Player {
public:
    SwapPlayer(double maxDistance, std::string color, char invariantKey);

    SwapPlayer(double maxDistance, std::string color, char invariantKey,
               const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

private:
    double maxDistance;
    std::string color;
    char invariantKey;

};


#endif //CUBE_SWAPPLAYER_H
