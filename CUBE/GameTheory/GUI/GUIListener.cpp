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
    QThread::msleep(15);
}
