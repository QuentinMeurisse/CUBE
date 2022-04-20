#include "GraphWorker.h"

GraphWorker::GraphWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent) : gameplay(std::move(gameplay))
                                                                                        , QObject(parent){}

void GraphWorker::process() {
    gameplay->play();
    emit finished();
}

void GraphWorker::stop() {
    gameplay->stop();
}

