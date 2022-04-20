#ifndef CUBE_ITERATIVEBETTERRANDOM_H
#define CUBE_ITERATIVEBETTERRANDOM_H


#include "IterativeGameplay.h"

#include <limits>

class IterativeBetterRandom : public IterativeGameplay{

public:
    IterativeBetterRandom(const std::vector<std::shared_ptr<Player>> &players, const std::shared_ptr<Game> &game,
                          unsigned int maxTimeWithoutImprovement,
                          unsigned int maxTotalTime = std::numeric_limits<unsigned int>::max());

    void play() override;

    std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) override;

    std::vector<std::shared_ptr<Player>> orderPLayers() override;


private:
    unsigned int maxTimeWithoutImprovement;
    unsigned int maxTotalTime;
};


#endif //CUBE_ITERATIVEBETTERRANDOM_H
