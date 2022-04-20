#include "CoModThread.h"

CoModThread::CoModThread(IterativeGameplay *gameplay, QObject *parent) : gameplay(gameplay), QThread(parent) {

}

void CoModThread::run() {
    gameplay->play();
    exec();
}
