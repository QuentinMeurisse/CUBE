#include "GraphListener.h"
#include "../GraphGame.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>


using namespace std;
using namespace this_thread;
using namespace chrono_literals;

void GraphListener::start(IterativeGameplay const &gameplay) {
}

void GraphListener::step(IterativeGameplay const &gameplay, Player const &p) {
    emit doUpdate();
    sleep_for(200ms);
}

void GraphListener::stop(IterativeGameplay const &gameplay) {
}

GraphListener::GraphListener(bool exported):exported(exported){

}

