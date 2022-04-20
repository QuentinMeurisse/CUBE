#ifndef CUBE_GRAPHWINDOW_H
#define CUBE_GRAPHWINDOW_H

#include "IterativeGameplay.h"
#include "../Graph.h"
#include "GraphFrame.h"

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>
#include <QMenu>
#include <QAction>
#include <memory>

class GraphWindow : public QMainWindow{
    Q_OBJECT
public:
    GraphWindow(std::shared_ptr<CoordGraph> g, std::shared_ptr<IterativeGameplay> gp, QWidget* parent = nullptr);

    void disableStartButton();
    void disableStopButton();

protected slots:
    void startGame();

    void zoomIn();
    void zoomOut();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
private:
    QPushButton* start;
    QPushButton* stop;
    GraphFrame* graphFrame;

    QMenu* zoomMenu;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* moveUpAct;
    QAction* moveDownAct;
    QAction* moveLeftAct;
    QAction* moveRightAct;

    int x_pos = 15;
    int y_pos = 15;
    int zoom_level = 30;

    std::shared_ptr<CoordGraph> graph;
    std::shared_ptr<IterativeGameplay> gameplay;

};


#endif //CUBE_GRAPHWINDOW_H
