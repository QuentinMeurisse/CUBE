#include "PrintListener.h"
#include "../UrbanBlockGame.h"
#include "IterativeGameplay.h"
#include "../Invariants/BuildingInvariant.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void PrintListener::start(IterativeGameplay const &gameplay) {
    startTime = high_resolution_clock::now();
}

void PrintListener::step(IterativeGameplay const &gameplay, Player const &p) {
    cout << p << "\n\t" << gameplay.getLastStrat() <<endl;
}

void PrintListener::stop(IterativeGameplay const &gameplay) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(gameplay.getGame());
    for (const shared_ptr<Player>& p : gameplay.getPlayers()){
        cout << *p << " payoff: " << p->payoff(gameplay.getGame()) << endl;
    }
    cout << "Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << " ms" <<endl;
    auto invs = ubg->getInvariants();
    shared_ptr<BuildingInvariant> bptr = dynamic_pointer_cast<BuildingInvariant>(invs['b']);
    //bptr->exportGraph("../graph.dot");
}
