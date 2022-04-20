#include "Timer.h"

#include <chrono>
#include <iostream>

using namespace std::chrono;

Timer::Timer(unsigned int maxTime) : maxTime(maxTime) {}

void Timer::start() {
    if (!started){
        started = true;
        startTime = high_resolution_clock::now();
    }
}

void Timer::stop() {
    started = false;
}

bool Timer::timeElapsed() {
    if (started)
        return duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() > maxTime;
    else
        return false;
}


