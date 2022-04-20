#ifndef CUBE_GUILISTENER_H
#define CUBE_GUILISTENER_H

#include "Listener.h"
#include <QObject>

class GUIListener: public QObject, public Listener {
    Q_OBJECT
public:
    void start(const IterativeGameplay &gameplay) override;

    void step(const IterativeGameplay &gameplay, const Player &p) override;

    void stop(const IterativeGameplay &gameplay) override;

signals:
    void doUpdate();

};


#endif //CUBE_GUILISTENER_H
