#include "TilingListener.h"
#include "../UrbanBlockGame.h"
#include "IterativeGameplay.h"
#include "../Invariants/BuildingInvariant.h"

#include <iostream>
#include <chrono>
#include <thread>


using namespace std;
using namespace this_thread;
using namespace chrono_literals;

void TilingListener::start(IterativeGameplay const &gameplay) {

}

void TilingListener::step(IterativeGameplay const &gameplay, Player const &p) {
    cout << p << "\n\t" << gameplay.getLastStrat() <<endl;
}

void TilingListener::stop(IterativeGameplay const &gameplay) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(gameplay.getGame());
    for (const shared_ptr<Player>& p : gameplay.getPlayers()){
        cout << *p << " payoff: " << p->payoff(gameplay.getGame()) << endl;
    }
    auto invs = ubg->getInvariants();
    shared_ptr<BuildingInvariant> bptr = dynamic_pointer_cast<BuildingInvariant>(invs['b']);
    //bptr->exportGraph("../graph.dot");
}
