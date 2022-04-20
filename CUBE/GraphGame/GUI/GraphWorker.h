#ifndef CUBE_GRAPHWORKER_H
#define CUBE_GRAPHWORKER_H

#include "IterativeGameplay.h"

#include <QObject>

class GraphWorker : public QObject{
    Q_OBJECT
public:
    explicit GraphWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent = nullptr);

public slots:
    void process();
    void stop();

signals:
    void finished();
private:
    std::shared_ptr<IterativeGameplay> gameplay;
};


#endif //CUBE_GRAPHWORKER_H
