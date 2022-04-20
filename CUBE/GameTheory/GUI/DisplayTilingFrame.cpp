#include "DisplayTilingFrame.h"

#include <utility>
#include <QScrollArea>

using namespace std;
DisplayTilingFrame::DisplayTilingFrame(std::shared_ptr<Tiling> t, int d, QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("CoMod");
    int w = min(800, 950);
    int h = min(800, 950);
    resize(w + 30, h + 30);
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    //setFixedSize(size());

    auto* scroll = new QScrollArea(centralWidget);
    scroll->setGeometry(10, 10, w+20, h+20);
    scroll->setFrameShape(QFrame::NoFrame);
    frame = new TilingFrame(std::move(t), d, centralWidget);
    scroll->setWidget(frame);
}
