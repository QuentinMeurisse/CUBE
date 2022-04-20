#ifndef CUBE_GRAPHLISTENER_H
#define CUBE_GRAPHLISTENER_H

#include "Listener.h"
#include "IterativeGameplay.h"

#include <QtCore/QObject>

class GraphListener : public QObject, public Listener {
    Q_OBJECT

public:
    explicit GraphListener(bool exported =false);

    void start(IterativeGameplay const &gameplay) override;

    void step(IterativeGameplay const &gameplay, Player const &p) override;

    void stop(IterativeGameplay const &gameplay) override;

signals:
    void doUpdate();

private:
    bool exported;

};


#endif //CUBE_GRAPHLISTENER_H
