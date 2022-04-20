#ifndef CUBE_BUILDINGS3DPERIMETERPLAYER_H
#define CUBE_BUILDINGS3DPERIMETERPLAYER_H

#include "Player.h"

#include <random>
#include <functional>

class Buildings3DPerimeterPlayer : public Player{
public:
    Buildings3DPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum, unsigned int const &rb,
                               unsigned int const &rg, double const& angle, double const& proj_height);

    Buildings3DPerimeterPlayer(unsigned int const &minum, unsigned int const &maxnum, unsigned int const &rb,
                               unsigned int const &rg, double const& angle, double const& proj_height,
                               std::function<bool (double, double)> f);

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
    double angle, projection_height;

};


#endif //CUBE_BUILDINGS3DPERIMETERPLAYER_H
