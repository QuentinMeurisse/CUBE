#include "GameWorker.h"

#include <utility>

GameWorker::GameWorker(std::shared_ptr<IterativeGameplay> gameplay, QObject *parent): gameplay(std::move(gameplay)), QObject(parent){}

void GameWorker::process() {
    gameplay->play();
    emit finished();

}

void GameWorker::stop() {
    gameplay->stop();
}
