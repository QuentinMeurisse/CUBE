#include "CoModWorker.h"

#include <utility>

CoModWorker::CoModWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent): gameplay(std::move(gameplay)), QObject(parent){}

void CoModWorker::process() {
    gameplay->play();
    emit finished();

}

void CoModWorker::stop() {
    gameplay->stop();
}
