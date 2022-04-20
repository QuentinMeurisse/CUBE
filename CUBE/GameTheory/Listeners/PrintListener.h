#ifndef CUBE_PRINTLISTENER_H
#define CUBE_PRINTLISTENER_H

#include "Listener.h"

#include <chrono>

class PrintListener : public Listener {

public:
    void start(IterativeGameplay const &gameplay) override;

    void step(IterativeGameplay const &gameplay, Player const &p) override;

    void stop(IterativeGameplay const &gameplay) override;

private:
    std::chrono::_V2::system_clock::time_point startTime{};

};


#endif //CUBE_PRINTLISTENER_H
