#ifndef CUBE_TILINGLISTENER_H
#define CUBE_TILINGLISTENER_H


#include "Listener.h"

class TilingListener : public Listener{
public:
    void start(IterativeGameplay const &gameplay) override;

    void step(IterativeGameplay const &gameplay, Player const &p) override;

    void stop(IterativeGameplay const &gameplay) override;


};


#endif //CUBE_TILINGLISTENER_H
