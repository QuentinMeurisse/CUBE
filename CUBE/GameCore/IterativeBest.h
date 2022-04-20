#ifndef CUBE_ITERATIVEBEST_H
#define CUBE_ITERATIVEBEST_H


#include "IterativeGameplay.h"

class IterativeBest: public IterativeGameplay {
public:
    IterativeBest(const std::vector<std::shared_ptr<Player>> &players, const std::shared_ptr<Game> &game);

    std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) override;

    std::vector<std::shared_ptr<Player>> orderPLayers() override;
};

class IterativeBestWithMemory : public IterativeBest{
public:
    IterativeBestWithMemory(const std::vector<std::shared_ptr<Player>> &players, const std::shared_ptr<Game> &game);

    std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) override;
};


#endif //CUBE_ITERATIVEBEST_H
