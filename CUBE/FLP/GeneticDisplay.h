#ifndef CUBE_GENETICDISPLAY_H
#define CUBE_GENETICDISPLAY_H

#include "GeneticProblem.h"

class GeneticDisplay : public GeneticListener{
public:
    void start(const GeneticProblem &gp) override;

    void step(const GeneticProblem &gp) override;

    void stop(const GeneticProblem &gp) override;
};


#endif //CUBE_GENETICDISPLAY_H
