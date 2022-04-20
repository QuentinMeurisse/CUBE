#ifndef CUBE_VARIABLEMOVEPLAYER_H
#define CUBE_VARIABLEMOVEPLAYER_H

#include "Player.h"

class VariableMovePlayer : public MultiStratPlayer{
public:
    VariableMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex);

    VariableMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex,
                       const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;

    std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) override;

    std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;

protected:
    void print(std::ostream &os) const override;

    double maxDistance;
    char invariantKey;
    unsigned int tokenIndex;

};

class VariableCooperativeMovePlayer : public VariableMovePlayer{
public:
    VariableCooperativeMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex);

    VariableCooperativeMovePlayer(int maxIndex, double maxDistance, char invariantKey, unsigned int tokenIndex,
                                  const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;
};

class VariableKCenterPlayer : public VariableMovePlayer{
public:
    VariableKCenterPlayer(int maxIndex, char invariantKey, unsigned int tokenIndex);

    VariableKCenterPlayer(int maxIndex, char invariantKey, unsigned int tokenIndex,
                          const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;
};


#endif //CUBE_VARIABLEMOVEPLAYER_H
