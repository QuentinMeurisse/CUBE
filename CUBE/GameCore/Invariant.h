#ifndef CUBE_INVARIANT_H
#define CUBE_INVARIANT_H

#include <memory>
#include "Strategy.h"

class Invariant {
public:
    explicit Invariant(std::shared_ptr<Game> g);

    Invariant(Invariant const& other);

    Invariant& operator=(Invariant const& other);

    virtual void update(const std::shared_ptr <Strategy> &strat) = 0;
    virtual void undo(const std::shared_ptr<Strategy> &strat) = 0;


protected:
    std::shared_ptr<Game> g;

};


#endif //CUBE_INVARIANT_H
