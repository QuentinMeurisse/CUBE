#ifndef CUBE_LISTENER_H
#define CUBE_LISTENER_H


#include "Player.h"

class IterativeGameplay;

class Listener {
public:
    virtual ~Listener();

    virtual void start(IterativeGameplay const& gameplay) = 0;
    virtual void step(IterativeGameplay const& gameplay, Player const& p) = 0;
    virtual void stop(IterativeGameplay const& gameplay) = 0;

};


#endif //CUBE_LISTENER_H
