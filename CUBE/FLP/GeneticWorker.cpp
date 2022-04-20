#include "GeneticWorker.h"

#include <utility>

using namespace std;

GeneticWorker::GeneticWorker(shared_ptr<GeneticFLP> problem, QObject *parent) : problem(move(problem)), QObject(parent){

}

void GeneticWorker::process() {
    problem->genetic_search();
    emit finished();
}
