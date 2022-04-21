#include "CUBELauncher.h"

#include <QVBoxLayout>

CUBELauncher::CUBELauncher(QWidget *parent): QMainWindow(parent) {
    setWindowTitle("CUBE");
    setFixedSize(600, 400);

    gf = new GameFrame;
    gcw = new GraphControlWindow;
    flpw = new FLPWindow;
    gapw = new GAPWindow;

    auto* centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: white");

    auto* label = new QLabel("Choose a module:", centralWidget);
    gameButton = new QPushButton("Game Theory", centralWidget);
    gameButton->setFixedSize(200, 30);
    graphButton = new QPushButton("Game Graph", centralWidget);
    graphButton->setFixedSize(200, 30);
    flpButton = new QPushButton("FLP", centralWidget);
    flpButton->setFixedSize(200, 30);
    gapButton = new QPushButton("GAP", centralWidget);
    gapButton->setFixedSize(200, 30);

    int x = 200;
    int y = 50;
    int incr = 60;
    label->move(100, 50);
    y += incr;
    gameButton->move(x, y);
    y += incr;
    graphButton->move(x, y);
    y += incr;
    flpButton->move(x, y);
    y += incr;
    gapButton->move(x, y);

    connect(gameButton, SIGNAL(clicked()), this, SLOT(selectGame()));
    connect(gf, SIGNAL(closed()), this, SLOT(reshowLauncher()));
    connect(graphButton, SIGNAL(clicked()), this, SLOT(selectGraph()));
    connect(gcw, SIGNAL(closed()), this, SLOT(reshowLauncher()));
    connect(flpButton, SIGNAL(clicked()), this, SLOT(selectFLP()));
    connect(flpw, SIGNAL(closed()), this, SLOT(reshowLauncher()));
    connect(gapButton, SIGNAL(clicked()), this, SLOT(selectGAP()));
    connect(gapw, SIGNAL(closed()), this, SLOT(reshowLauncher()));

}

void CUBELauncher::selectGame() {
    this->hide();
    gf->show();
}

void CUBELauncher::selectGraph() {
    this->hide();
    gcw->show();
}

void CUBELauncher::selectFLP() {
    this->hide();
    flpw->show();
}

void CUBELauncher::selectGAP() {
    this->hide();
    gapw->show();
}

void CUBELauncher::reshowLauncher() {
    this->show();
}



