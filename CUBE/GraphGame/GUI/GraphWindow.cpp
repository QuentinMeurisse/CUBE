#include "GraphWindow.h"
#include "GraphListener.h"
#include "GraphWorker.h"

#include <QInputDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include <QMenuBar>
#include <iostream>
#include <utility>
#include <memory>


using namespace std;

GraphWindow::GraphWindow(std::shared_ptr<CoordGraph> g, std::shared_ptr<IterativeGameplay> gp, QWidget *parent) : QMainWindow(parent), graph(std::move(g)), gameplay(std::move(gp)){
    setWindowTitle("Graph");
    resize(1920, 1080);
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    centralWidget->setStyleSheet("background-color: white");

    graphFrame = new GraphFrame(graph, x_pos, y_pos, zoom_level, zoom_level/3., centralWidget);

    start = new QPushButton("START", centralWidget);
    start->move(1500, 10);
    start->setStyleSheet("QPushButton:hover { background-color: green; }");
    start->setFixedSize(300, 30);
    stop = new QPushButton("STOP", centralWidget);
    stop->move(1500, 50);
    stop->setStyleSheet("QPushButton:hover { background-color: red; }");
    stop->setFixedSize(300, 30);

    zoomInAct = new QAction(tr("&Zoom in"), this);
    zoomInAct->setShortcuts({QKeySequence::ZoomIn, QKeySequence(Qt::CTRL + Qt::Key_Equal)});
    zoomOutAct = new QAction(tr("&Zoom out"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    moveUpAct = new QAction(tr("Move Up"), this);
    moveUpAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    moveDownAct = new QAction(tr("Move Down"), this);
    moveDownAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    moveLeftAct = new QAction(tr("Move Left"), this);
    moveLeftAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    moveRightAct = new QAction(tr("Move Right"), this);
    moveRightAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));
    connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));
    connect(moveLeftAct, SIGNAL(triggered()), this, SLOT(moveLeft()));
    connect(moveRightAct, SIGNAL(triggered()), this, SLOT(moveRight()));

    zoomMenu = menuBar()->addMenu(tr("&Zoom"));
    zoomMenu->addAction(zoomInAct);
    zoomMenu->addAction(zoomOutAct);
    zoomMenu->addAction(moveUpAct);
    zoomMenu->addAction(moveDownAct);
    zoomMenu->addAction(moveLeftAct);
    zoomMenu->addAction(moveRightAct);

    connect(start, SIGNAL(clicked()), this, SLOT(startGame()));
}

void GraphWindow::startGame() {
    shared_ptr<GraphListener> l = make_shared<GraphListener>();
    gameplay->addListener(l);
    connect(l.get(), SIGNAL(doUpdate()), graphFrame, SLOT(repaint()));

    auto* qThread = new QThread;
    auto* worker = new GraphWorker(gameplay);
    worker->moveToThread(qThread);
    connect(stop, SIGNAL(clicked()), worker, SLOT(stop()), Qt::DirectConnection);
    connect(qThread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), qThread, SLOT(quit()));
    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
    connect(qThread, SIGNAL (finished()), qThread, SLOT (deleteLater()));

    qThread->start();

}

void GraphWindow::disableStartButton() {
    start->setEnabled(false);
}

void GraphWindow::disableStopButton() {
    stop->setEnabled(false);
}

void GraphWindow::zoomIn() {
    if (zoom_level < 300){
        zoom_level += 3;
        graphFrame->setDilatation(zoom_level);
        graphFrame->setRadius(zoom_level / 3.);
        graphFrame->repaint();
    }
}

void GraphWindow::zoomOut() {
    if (zoom_level > 3){
        zoom_level -= 3;
        graphFrame->setDilatation(zoom_level);
        graphFrame->setRadius(zoom_level / 3.);
        graphFrame->repaint();
    }

}

void GraphWindow::moveUp() {
    y_pos -= 5;
    graphFrame->setYtranslation(y_pos);
    graphFrame->repaint();
}

void GraphWindow::moveDown() {
    y_pos += 5;
    graphFrame->setYtranslation(y_pos);
    graphFrame->repaint();
}

void GraphWindow::moveLeft() {
    x_pos -= 5;
    graphFrame->setXtranslation(x_pos);
    graphFrame->repaint();
}

void GraphWindow::moveRight() {
    x_pos += 5;
    graphFrame->setXtranslation(x_pos);
    graphFrame->repaint();
}
