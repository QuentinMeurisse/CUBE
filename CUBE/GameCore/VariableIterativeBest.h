#ifndef CUBE_VARIABLEITERATIVEBEST_H
#define CUBE_VARIABLEITERATIVEBEST_H

#include "IterativeGameplay.h"

class VariableIterativeBest : public IterativeGameplay{
public:
    VariableIterativeBest(const std::vector<std::shared_ptr<Player>> &players, const std::shared_ptr<Game> &game);

    std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) override;

    std::vector<std::shared_ptr<Player>> orderPLayers() override;


};

class VariableIterativeBestWithMemory : public VariableIterativeBest{
public:
    VariableIterativeBestWithMemory(const std::vector<std::shared_ptr<Player>> &players,
                                    const std::shared_ptr<Game> &game);

    std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) override;


};


#endif //CUBE_VARIABLEITERATIVEBEST_H
