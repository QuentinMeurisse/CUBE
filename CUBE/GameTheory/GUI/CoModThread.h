#ifndef CUBE_COMODTHREAD_H
#define CUBE_COMODTHREAD_H


#include <QThread>
#include "IterativeGameplay.h"

//todo: delete
class CoModThread : public QThread{
public:
    explicit CoModThread(IterativeGameplay* gameplay, QObject *parent= nullptr);

private:
    IterativeGameplay* gameplay;
    void run() override;
};


#endif //CUBE_COMODTHREAD_H
