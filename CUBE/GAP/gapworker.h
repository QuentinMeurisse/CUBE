#ifndef CUBE_GAPWORKER_H
#define CUBE_GAPWORKER_H

#include "gap_config.h"
#include <QObject>
#include <QString>

class GAPWorker : public QObject{
    Q_OBJECT
public:
    explicit GAPWorker(Config  c, QObject* parent = nullptr);

public slots:
            void process();

signals:
        void finished();
        void infoChanged(QString info);
private:
    Config config;

};



#endif //CUBE_GAPWORKER_H
