#ifndef CUBE_MOVEPLAYER_H
#define CUBE_MOVEPLAYER_H

#include "Player.h"

class MovePlayer : public Player {
public:
    MovePlayer(double maxDistance, char key, unsigned int tokenIndex);

    MovePlayer(double maxDistance, char key, unsigned int tokenIndex, std::function<bool(double, double)> better);

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

class CooperativeMovePlayer : public MovePlayer{
public:
    CooperativeMovePlayer(double maxDistance, char key, unsigned int tokenIndex);

    CooperativeMovePlayer(double maxDistance, char key, unsigned int tokenIndex,
                          const std::function<bool(double, double)> &better);


    double payoff(std::shared_ptr<Game> game) override;
};

class CountMovePlayer : public MovePlayer{
public:
    CountMovePlayer(double maxDistance, char key, unsigned int tokenIndex);

    CountMovePlayer(double maxDistance, char key, unsigned int tokenIndex,
                    const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;
};

class KCenterPlayer : public MovePlayer{
public:
    KCenterPlayer(char key, unsigned int tokenIndex);

    KCenterPlayer(char key, int tokenIndex,
                  const std::function<bool(double, double)> &better);

    double payoff(std::shared_ptr<Game> game) override;

    bool isMaximizing() override;


};


#endif //CUBE_MOVEPLAYER_H
