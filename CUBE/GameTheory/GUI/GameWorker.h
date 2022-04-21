#ifndef CUBE_GAMEWORKER_H
#define CUBE_GAMEWORKER_H

#include "IterativeGameplay.h"

#include <QObject>

class GameWorker : public QObject {
    Q_OBJECT
public:
    explicit GameWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent = nullptr);

public slots:
    void process();
    void stop();

signals:
    void finished();

private:
    std::shared_ptr<IterativeGameplay> gameplay;

};


#endif //CUBE_GAMEWORKER_H
