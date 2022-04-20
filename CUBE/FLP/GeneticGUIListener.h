#ifndef CUBE_GENETICGUILISTENER_H
#define CUBE_GENETICGUILISTENER_H

#include "GeneticProblem.h"
#include "Graph.h"

#include <QtCore/QObject>
#include <memory>


class GeneticGUIListener : public QObject, public GeneticListener{
    Q_OBJECT
public:
    GeneticGUIListener(std::shared_ptr<CoordGraph> graph, std::vector<uint64_t> whites, bool slow_mode = false);

    void start(const GeneticProblem &gp) override;

    void step(const GeneticProblem &gp) override;

    void stop(const GeneticProblem &gp) override;

signals:
    void doUpdate();
    void generationChanged(int k);
    void fitnessChanged(double x);
    void factoriesChanged(long num_0, long num_1);


private:
    std::shared_ptr<CoordGraph> g;
    std::vector<uint64_t> white_nodes;
    bool slow;
};


#endif //CUBE_GENETICGUILISTENER_H
