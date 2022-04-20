#ifndef CUBE_COMODWORKER_H
#define CUBE_COMODWORKER_H

#include "IterativeGameplay.h"

#include <QObject>

class CoModWorker : public QObject {
    Q_OBJECT
public:
    explicit CoModWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent = nullptr);

public slots:
    void process();
    void stop();

signals:
    void finished();

private:
    std::shared_ptr<IterativeGameplay> gameplay;

};


#endif //CUBE_COMODWORKER_H
