#include "GUIListener.h"
#include "../UrbanBlockGame.h"
#include "IterativeGameplay.h"

#include <memory>
#include <iostream>
#include <QThread>

using namespace std;
void GUIListener::start(const IterativeGameplay &gameplay) {

}

void GUIListener::step(const IterativeGameplay &gameplay, const Player &p) {
    emit doUpdate();
    QThread::msleep(15);
}

void GUIListener::stop(const IterativeGameplay &gameplay) {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(gameplay.getGame());
    for (const shared_ptr<Player>& p : gameplay.getPlayers()){
        cout << *p << " payoff: " << p->payoff(gameplay.getGame()) << endl;
    }
    QThread::msleep(15);
}
