#ifndef CUBE_PLAYER_H
#define CUBE_PLAYER_H


#include "Game.h"
#include "Strategy.h"
#include "FiniteMemory.h"

#include <vector>
#include <memory>
#include <ostream>
#include <functional>
#include <random>

class Player {
public:

    explicit Player(std::function<bool(double, double)> better);

    Player();

    virtual ~Player();

    virtual double payoff(std::shared_ptr<Game> game) = 0;
    virtual std::vector<std::shared_ptr<Strategy>> getAllStrat(std::shared_ptr<Game> game) = 0;
    virtual std::shared_ptr<Strategy> getRandomStrat(std::shared_ptr<Game> game) = 0;
    virtual bool isMaximizing() = 0;

    bool applyBetter(double x, double y);

    /**
     * Used to the rng used to choose random strategy. That allow us to have many random gameplay with players that
     * have the same "random" behavior
     */
    void resetRNG();

    [[nodiscard]] unsigned int getSeed() const;

    void setSeed(unsigned int num);

    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    void addToMemory(const std::shared_ptr<Strategy>& strat);

    bool memoryContainsTooMuch(const std::shared_ptr<Strategy>& strat);

    [[nodiscard]] const FiniteMemory<Strategy> &getMemory() const;

    void setMemory(size_t max_size, int max_duplications);

protected:
    virtual void print(std::ostream &os) const =0;

    std::function<bool (double, double)> better;
    unsigned int seed;
    std::mt19937 gen;
    FiniteMemory<Strategy> memory;
};


class MultiStratPlayer : public Player{
public:
    MultiStratPlayer(const std::function<bool(double, double)> &better, int maxIndex);

    explicit MultiStratPlayer(int maxIndex);

    [[nodiscard]] int getStratIndex() const;

    void setStratIndex(int stratIndex);

    void increaseStratIndex();

    [[nodiscard]] int getMaxIndex() const;


protected:
    int strat_index = 1;
    int max_index;
};

#endif //CUBE_PLAYER_H
