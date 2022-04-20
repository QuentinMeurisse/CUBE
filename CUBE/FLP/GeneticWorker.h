#ifndef CUBE_GENETICWORKER_H
#define CUBE_GENETICWORKER_H

#include "GeneticFLP.h"

#include <QtCore/QObject>
#include <memory>


class GeneticWorker : public  QObject{
    Q_OBJECT
public:
    explicit GeneticWorker(std::shared_ptr<GeneticFLP> problem , QObject* parent = nullptr);

public slots:
    void process();

signals:
    void finished();

private:
    std::shared_ptr<GeneticFLP> problem;

};


#endif //CUBE_GENETICWORKER_H
