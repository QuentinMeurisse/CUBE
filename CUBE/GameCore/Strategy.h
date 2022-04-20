#ifndef CUBE_STRATEGY_H
#define CUBE_STRATEGY_H


#include <memory>
#include <ostream>
#include "Game.h"

class Strategy {
public:
    virtual ~Strategy();

    virtual void select(std::shared_ptr<Game> g) = 0;
    virtual void unselect(std::shared_ptr<Game> g) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Strategy &strategy);

    bool operator==(Strategy const& other) const;

protected:
    virtual void print(std::ostream &os) const =0;

    [[nodiscard]] virtual bool equal(Strategy const& other) const = 0;
};


#endif //CUBE_STRATEGY_H
