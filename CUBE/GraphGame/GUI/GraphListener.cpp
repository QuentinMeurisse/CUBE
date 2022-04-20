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
    cout << "start" << endl;
}

void GraphListener::step(IterativeGameplay const &gameplay, Player const &p) {
    cout << p << "\n\t" << gameplay.getLastStrat() <<endl;
    emit doUpdate();
    sleep_for(200ms);
}

void GraphListener::stop(IterativeGameplay const &gameplay) {
    for (const shared_ptr<Player>& p : gameplay.getPlayers()){
        cout << *p << " payoff: " << p->payoff(gameplay.getGame()) << endl;
    }
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(gameplay.getGame());
    cout << "finish" << endl;
}

GraphListener::GraphListener(bool exported):exported(exported){

}

