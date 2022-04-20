#ifndef CUBE_HEIGHTPLAYER_H
#define CUBE_HEIGHTPLAYER_H

#include "Player.h"
#include "HeightConstraint.h"

class HeightPlayer : public Player{
public:
    explicit HeightPlayer(double floor, double angle, double height_projection);

    HeightPlayer(double floor, double angle, double height_projection,
                 const std::function<bool(double, double)> &better);

private:
    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

    void print(std::ostream &os) const override;


private:
    double floor;
    HeightConstraint constraint = HeightConstraint(0, 0);
};


#endif //CUBE_HEIGHTPLAYER_H
