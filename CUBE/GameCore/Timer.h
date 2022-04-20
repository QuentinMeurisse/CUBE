#ifndef CUBE_TIMER_H
#define CUBE_TIMER_H

#include <chrono>

class Timer {

public:
    /**
     *
     * @param maxTime the time value for timer supposed to be in milliseconds
     */
    explicit Timer(unsigned int maxTime);

    void start();

    void stop();

    bool timeElapsed();

private:
    unsigned int maxTime;
    std::chrono::_V2::system_clock::time_point startTime{};
    bool started = false;
};


#endif //CUBE_TIMER_H
