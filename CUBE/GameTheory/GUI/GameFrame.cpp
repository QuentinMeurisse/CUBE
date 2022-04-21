#include "GameFrame.h"
#include "../SquareTiling.h"
#include "../HexagonalTiling.h"
#include "../TriangularTiling.h"
#include "IterativeBest.h"
#include "IterativeBetterRandom.h"
#include "../UrbanBlockGame.h"
#include "../Invariants/BuildingInvariant.h"
#include "../Invariants/GreenSpaceInvariant.h"
#include "Player.h"
#include "../Players/PorosityPlayer.h"
#include "../Players/NumBuildingPlayer.h"
#include "../Players/BuildingsPerimeterPlayer.h"
#include "../Players/GreenSpaceRatePlayer.h"
#include "../Players/NumGreenSpacesPlayer.h"
#include "../GameTheoryUtility.h"
#include "../Listeners/TilingListener.h"
#include "GameWorker.h"
#include "GUIListener.h"
#include "../3D/Building3DInvariant.h"
#include "../3D/GreenSpaces3DInvariant.h"
#include "../3D/Buildings3DPerimeterPlayer.h"
#include "../3D/NumBuilding3DPlayer.h"
#include "../3D/HeightPlayer.h"

#include <utility>
#include <memory>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QString>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

GameFrame::GameFrame(QWidget *parent) : QMainWindow(parent){
    setWindowTitle("Game Theory");
    resize(1920, 1000);
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    //setFixedSize(size());

    tilingFrame = new TilingFrame(centralWidget);
    tilingFrame->move(10, 10);

    auto* tilingGroup = new QGroupBox("Tiling configuration", centralWidget);
    auto* setupTilingWidget = new QWidget(tilingGroup);
    tilingW = new QLineEdit("50", setupTilingWidget);
    tilingW->setFixedWidth(50);
    tilingH = new QLineEdit("50", setupTilingWidget);
    tilingH->setFixedWidth(50);
    tilingA = new QLineEdit("1", setupTilingWidget);
    tilingA->setFixedWidth(50);

    auto* tilingLayout = new QFormLayout;
    tilingLayout->addRow("Width:", tilingW);
    tilingLayout->addRow("Height:", tilingH);
    tilingLayout->addRow("Cells area:", tilingA);
    setupTilingWidget->setLayout(tilingLayout);


    setupTilingButton = new QPushButton("SETUP", tilingGroup);
    squareTiling = new QRadioButton(tr("Square tiling"), tilingGroup);
    squareTiling->setChecked(true);
    hexagonTiling = new QRadioButton(tr("Hexagonal tiling"), tilingGroup);
    triangleTiling = new QRadioButton(tr("Triangular tiling"), tilingGroup);

    auto* setupTilingLayout = new QVBoxLayout;
    setupTilingLayout->addWidget(setupTilingWidget);
    setupTilingLayout->addWidget(squareTiling);
    setupTilingLayout->addWidget(hexagonTiling);
    setupTilingLayout->addWidget(triangleTiling);
    setupTilingLayout->addWidget(setupTilingButton);
    tilingGroup->setLayout(setupTilingLayout);
    tilingGroup->move(800, 10);

    auto* gameGroup = new QGroupBox("Game", centralWidget);

    auto* gameplayGroup = new QGroupBox("Gameplay", gameGroup);
    deterministGameplay = new QRadioButton(tr("Determinist gameplay"), gameplayGroup);
    randomGameplay = new QRadioButton(tr("Random gameplay"), gameplayGroup);
    startFromRandom = new QCheckBox("Start from random solution");
    auto* gameplayLayout = new QVBoxLayout;
    gameplayLayout->addWidget(deterministGameplay);
    gameplayLayout->addWidget(randomGameplay);
    gameplayLayout->addWidget(startFromRandom);
    gameplayLayout->addStretch(1);
    gameplayGroup->setLayout(gameplayLayout);

    maxTimeWidget = new QWidget(gameplayGroup);
    maxTimeLine = new QLineEdit("5000", maxTimeWidget);
    maxTimeLine->setFixedWidth(75);
    auto* maxTimeLayout = new QFormLayout;
    maxTimeLayout->addRow("Max time without improvement (ms):", maxTimeLine);
    maxTimeWidget->setLayout(maxTimeLayout);
    gameplayLayout->addWidget(maxTimeWidget);

    QSizePolicy sp = maxTimeWidget->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    maxTimeWidget->setSizePolicy(sp);
    maxTimeWidget->setHidden(true);

    auto* playersGroup = new QGroupBox("Players", gameplayGroup);

    porosityCheck = new QCheckBox(tr("Porosity"), playersGroup);

    numBuildingCheck = new QCheckBox("Num buildings", playersGroup);
    auto* minNumB = new QLabel("min:", playersGroup);
    minBuildingLine = new QLineEdit("5", playersGroup);
    minBuildingLine->setFixedWidth(50);
    minBuildingLine->setEnabled(false);
    auto* maxNumB = new QLabel("max:", playersGroup);
    maxBuildingLine = new QLineEdit("10", playersGroup);
    maxBuildingLine->setFixedWidth(50);
    maxBuildingLine->setEnabled(false);

    buildingsPerimeterCheck = new QCheckBox("Buildings perimeter");
    auto* minPerim = new QLabel("min:", playersGroup);
    minPerimLine = new QLineEdit("75", playersGroup);
    minPerimLine->setFixedWidth(50);
    minPerimLine->setEnabled(false);
    auto* maxPerim = new QLabel("max:", playersGroup);
    maxPerimLine = new QLineEdit("500", playersGroup);
    maxPerimLine->setFixedWidth(50);;
    maxPerimLine->setEnabled(false);

    greenSpacesRateCheck = new QCheckBox("Green spaces rate", playersGroup);
    auto* greenSpace = new QLabel("rate:");
    greenSpaceRateLine = new QLineEdit("0.2", playersGroup);
    greenSpaceRateLine->setFixedWidth(50);
    greenSpaceRateLine->setEnabled(false);

    numGreenSpacesCheck = new QCheckBox("Num green spaces", playersGroup);
    auto* minGreenSpace = new QLabel("min:", playersGroup);
    minGreenSpacesLine = new QLineEdit("3", playersGroup);
    minGreenSpacesLine->setFixedWidth(50);
    minGreenSpacesLine->setEnabled(false);
    auto* maxGreenSpace = new QLabel("max:", playersGroup);
    maxGreenSpacesLine = new QLineEdit("5",playersGroup);
    maxGreenSpacesLine->setFixedWidth(50);
    maxGreenSpacesLine->setEnabled(false);

    QLabel* heightPlayer = new QLabel("Buildings height", playersGroup);
    auto* floorHeight = new QLabel("floor height: ", playersGroup);
    floorHeightLine = new QLineEdit("3", playersGroup);
    floorHeightLine->setFixedWidth(50);
    floorHeightLine->setEnabled(false);

    auto* playerLayout = new QGridLayout;
    playerLayout->addWidget(porosityCheck, 0, 0);

    playerLayout->addWidget(numBuildingCheck, 1, 0);
    playerLayout->addWidget(minNumB, 1, 1);
    playerLayout->addWidget(minBuildingLine, 1, 2);
    playerLayout->addWidget(maxNumB, 1, 3);
    playerLayout->addWidget(maxBuildingLine, 1, 4);

    playerLayout->addWidget(buildingsPerimeterCheck, 2, 0);
    playerLayout->addWidget(minPerim, 2, 1);
    playerLayout->addWidget(minPerimLine, 2, 2);
    playerLayout->addWidget(maxPerim, 2, 3);
    playerLayout->addWidget(maxPerimLine, 2, 4);

    playerLayout->addWidget(greenSpacesRateCheck, 4, 0);
    playerLayout->addWidget(greenSpace, 4, 1);
    playerLayout->addWidget(greenSpaceRateLine, 4, 2);

    playerLayout->addWidget(numGreenSpacesCheck, 3, 0);
    playerLayout->addWidget(minGreenSpace, 3, 1);
    playerLayout->addWidget(minGreenSpacesLine, 3, 2);
    playerLayout->addWidget(maxGreenSpace, 3, 3);
    playerLayout->addWidget(maxGreenSpacesLine, 3, 4);

    playerLayout->addWidget(heightPlayer, 5, 0);
    playerLayout->addWidget(floorHeight, 5, 1);
    playerLayout->addWidget(floorHeightLine, 5, 2);

    playersGroup->setLayout(playerLayout);

    auto* radiusGroup = new QGroupBox("Constraints");
    rbLine = new QLineEdit("4", radiusGroup);
    rbLine->setFixedWidth(50);
    rgLine = new QLineEdit("1", radiusGroup);
    rgLine->setFixedWidth(50);

    angleLine = new QLineEdit("45", radiusGroup);
    angleLine->setFixedWidth(50);
    angleLine->setEnabled(false);
    heightProjectionLine = new QLineEdit("1", radiusGroup);
    heightProjectionLine->setFixedWidth(50);
    heightProjectionLine->setEnabled(false);

    auto* radiusLayout = new QFormLayout;
    radiusLayout->addRow("Distance between buildings:", rbLine);
    radiusLayout->addRow("Distance between  green spaces:", rgLine);
    radiusLayout->addRow("Projection angle:", angleLine);
    radiusLayout->addRow("Projection height:", heightProjectionLine);
    radiusGroup->setLayout(radiusLayout);

    use3D = new QCheckBox("Use 3D mode", gameGroup);
    save3DButton = new QPushButton("Save 3D configuration", gameGroup);
    save3DButton->setEnabled(false);

    auto* gameLayout = new QGridLayout;
    gameLayout->addWidget(gameplayGroup, 0, 0);
    gameLayout->addWidget(playersGroup, 0, 1);
    gameLayout->addWidget(radiusGroup, 1, 0);
    gameLayout->addWidget(use3D, 1, 1);
    gameLayout->addWidget(save3DButton, 2, 1);
    gameGroup->setLayout(gameLayout);

    gameGroup->move(800, 300);


    startAndRestartButtons = new QStackedWidget(centralWidget);

    auto* startButton = new QPushButton("START", startAndRestartButtons);
    startButton->resize(300, 50);

    auto* restartButton = new QPushButton("RESTART", startAndRestartButtons);
    startButton->resize(300, 50);

    startAndRestartButtons->addWidget(startButton);
    startAndRestartButtons->addWidget(restartButton);
    startAndRestartButtons->resize(300, 50);
    startAndRestartButtons->move(800, 800);

    resetSolutionButton = new QPushButton("RESET SOLUTION", centralWidget);
    resetSolutionButton->resize(300, 50);
    resetSolutionButton->move(1150, 800);

    stopButton = new QPushButton("STOP", centralWidget);
    stopButton->resize(300, 50);
    stopButton->move(1500, 800);
    stopButton->setStyleSheet("QPushButton:hover { background-color: red; }\n");

    auto lockGroup = new QGroupBox("Lock cells", centralWidget);
    loadLockedCellsButton = new QPushButton("Load locked cells", lockGroup);
    loadLockedCellsButton->setEnabled(false);
    lockedCellsLine = new QLineEdit(lockGroup);
    lockedCellsLine->setEnabled(false);
    lockedCellsLine->setFixedWidth(200);
    lockCellsButton = new QPushButton("Lock cells", lockGroup);
    lockCellsButton->setEnabled(false);
    auto* lockLayout = new QGridLayout;
    lockLayout->addWidget(loadLockedCellsButton, 0, 0);
    lockLayout->addWidget(lockedCellsLine, 0, 1);
    lockLayout->addWidget(lockCellsButton, 1, 0, 1, 2);
    lockGroup->setLayout(lockLayout);
    lockGroup->move(1300, 10);

    connect(setupTilingButton, SIGNAL(clicked()), this, SLOT(initTiling()));

    connect(numBuildingCheck, SIGNAL(stateChanged(int)), this, SLOT(updateNumBuildings(int)));
    connect(buildingsPerimeterCheck, SIGNAL(stateChanged(int)), this, SLOT(updateBuildingPerim(int)));
    connect(greenSpacesRateCheck, SIGNAL(stateChanged(int)), this, SLOT(updateGreenSpacesrate(int)));
    connect(numGreenSpacesCheck, SIGNAL(stateChanged(int)), this, SLOT(updateNumGreenSpaces(int)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(initGame()));
    connect(resetSolutionButton, SIGNAL(clicked()), this, SLOT(resetSolution()));
    connect(restartButton, SIGNAL(clicked()), this, SLOT(restartGameplay()));

    connect(randomGameplay, SIGNAL(toggled(bool)), this, SLOT(showMaxTimeField(bool)));

    connect(loadLockedCellsButton, SIGNAL(clicked()), this, SLOT(loadLock()));
    connect(lockCellsButton, SIGNAL(clicked()), this, SLOT(lockCells()));

    connect(use3D, SIGNAL(stateChanged(int)), this, SLOT(active3DMode(int)));
    connect(save3DButton, SIGNAL(clicked()), this, SLOT(save3DConfig()));

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



}

void GameFrame::initTiling() {
    QString wstr = tilingW->text();
    QString hstr = tilingH->text();
    QString astr = tilingA->text();
    QString errorDetails;

    if(wstr.isEmpty())
        errorDetails.append("- Tiling's width is not set.\n");
    if (hstr.isEmpty())
        errorDetails.append("- Tiling's height is not set.\n");
    if (astr.isEmpty())
        errorDetails.append("- Cells area is not set.\n");

    if (errorDetails.isEmpty()) {
        int w = wstr.toInt();
        int h = hstr.toInt();
        double area = astr.toDouble();

        shared_ptr<Tiling> t;
        if (squareTiling->isChecked())
            t = make_shared<SquareTiling>(w, h, area);
        if (hexagonTiling->isChecked())
            t = make_shared<HexagonalTiling>(w, h, area);
        if (triangleTiling->isChecked())
            t = make_shared<TriangularTiling>(w, h, area);
        t->init();
        tilingFrame->setT(t);

        tilingFrame->repaint();
        startAndRestartButtons->setCurrentIndex(0);
        loadLockedCellsButton->setEnabled(true);
        lockedCellsLine->setEnabled(true);
        lockCellsButton->setEnabled(true);
    }
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(errorDetails);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
}

void GameFrame::updateNumBuildings(int state){
    minBuildingLine->setEnabled(state == Qt::Checked);
    maxBuildingLine->setEnabled(state == Qt::Checked);
}

void GameFrame::updateBuildingPerim(int state){
    minPerimLine->setEnabled(state == Qt::Checked);
    maxPerimLine->setEnabled(state == Qt::Checked);
}

void GameFrame::updateGreenSpacesrate(int state){
    greenSpaceRateLine->setEnabled(state == Qt::Checked);
}

void GameFrame::updateNumGreenSpaces(int state){
    minGreenSpacesLine->setEnabled(state == Qt::Checked);
    maxGreenSpacesLine->setEnabled(state == Qt::Checked);
}

void GameFrame::active3DMode(int state) {
    floorHeightLine->setEnabled(state == Qt::Checked);
    angleLine->setEnabled(state == Qt::Checked);
    save3DButton->setEnabled(state == Qt::Checked);
    heightProjectionLine->setEnabled(state == Qt::Checked);
    squareTiling->setChecked(true);
    hexagonTiling->setEnabled(state != Qt::Checked);
    triangleTiling->setEnabled(state != Qt::Checked);
    if (tilingFrame->getT() != nullptr)
        initTiling();
}

void GameFrame::initGame() {
    shared_ptr<UrbanBlockGame> game = make_shared<UrbanBlockGame>(tilingFrame->getT());

    if (use3D->isChecked()){
        QString floortxt = floorHeightLine->text();
        if (floortxt.isEmpty()){
            QMessageBox messageBox;
            messageBox.setText("Floor height is not configured !");
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.exec();
        }
        double fh = floortxt.toDouble();
        shared_ptr<Building3DInvariant> binv = make_shared<Building3DInvariant>(game, fh);
        game->addInvariant(binv, 'b');
        shared_ptr<GreenSpaces3DInvariant> ginv = make_shared<GreenSpaces3DInvariant>(game);
        game->addInvariant(ginv, 'g');
    }else {
        shared_ptr<BuildingInvariant> binv = make_shared<BuildingInvariant>(game);
        game->addInvariant(binv, 'b');

        shared_ptr<GreenSpaceInvariant> ginv = make_shared<GreenSpaceInvariant>(game);
        game->addInvariant(ginv, 'g');
    }
    initGameplay(game);

}

void GameFrame::restartGameplay() {
    shared_ptr<Game> game = gameplay->getGame();
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(game);
    initGameplay(ubg);
}

void GameFrame::enableAll() {
    for(auto* widget : findChildren<QWidget *>())
        widget->setEnabled(true);
    startAndRestartButtons->setCurrentIndex(1);
    updateBuildingPerim(buildingsPerimeterCheck->checkState());
    updateNumBuildings(numBuildingCheck->checkState());
    updateGreenSpacesrate(greenSpacesRateCheck->checkState());
    updateNumGreenSpaces(numGreenSpacesCheck->checkState());
    int state = use3D->checkState();
    floorHeightLine->setEnabled(state == Qt::Checked);
    angleLine->setEnabled(state == Qt::Checked);
    save3DButton->setEnabled(state == Qt::Checked);
    heightProjectionLine->setEnabled(state == Qt::Checked);
    squareTiling->setChecked(true);
    hexagonTiling->setEnabled(state != Qt::Checked);
    triangleTiling->setEnabled(state != Qt::Checked);

}

void GameFrame::resetSolution() {
    shared_ptr<Tiling> t = tilingFrame->getT();
    for (int i = 0; i < t->numCells(); ++i) {
        t->setColor(i, "#ffffff");
        t->setComponent(i, 0);
    }
    tilingFrame->repaint();
    startAndRestartButtons->setCurrentIndex(0);
}

void GameFrame::showMaxTimeField(bool state) {
    maxTimeWidget->setHidden(not state);
}


void GameFrame::initGameplay(const shared_ptr<UrbanBlockGame> &game) {
    QString errorDetails;
    if (not tilingFrame->getT())
        errorDetails.append("- The tiling is not set.\n");

    QString rbstr = rbLine->text();
    if (rbstr.isEmpty())
        errorDetails.append("- The distance between buildings is not set.\n");
    unsigned int rb = rbstr.toUInt();
    QString rgstr = rgLine->text();
    if (rgstr.isEmpty())
        errorDetails.append("- The distance between green spaces is not set.\n");
    unsigned int rg = rgstr.toUInt();

    if (errorDetails.isEmpty() and startFromRandom->isChecked()){
        if (numGreenSpacesCheck->isChecked() or greenSpacesRateCheck->isChecked())
            GameTheoryUtility::generateRandomGameWithGreen(game, rb, rg);
        else
            GameTheoryUtility::generateRandomGame(game, rb);
    }

    vector<shared_ptr<Player>> players = use3D->isChecked() ? init3DPlayers(rb, rg) : init2DPlayers(rb, rg);

    if (players.empty())
        errorDetails.append("- No player is selected\n");

    if (deterministGameplay->isChecked()){
        gameplay = make_shared<IterativeBest>(players, game);
    }
    else if (randomGameplay->isChecked()){
        QString maxTimeStr = maxTimeLine->text();
        if (maxTimeStr.isEmpty())
            errorDetails.append("- Max time without improvement is not set\n");
        unsigned int maxTime = maxTimeStr.toUInt();
        gameplay = make_shared<IterativeBetterRandom>(players, game, maxTime);
    }
    else
        errorDetails.append("- No gameplay is selected.");

    if (errorDetails.isEmpty()) {
        shared_ptr<GUIListener> l = make_shared<GUIListener>();
        gameplay->addListener(l);
        connect(l.get(), SIGNAL(doUpdate()), tilingFrame, SLOT(repaint()));
        tilingFrame->repaint();

        for (auto *widget : findChildren<QGroupBox *>())
            widget->setEnabled(false);

        startAndRestartButtons->setEnabled(false);
        resetSolutionButton->setEnabled(false);

        auto* qThread = new QThread;
        auto* worker = new GameWorker(gameplay);
        worker->moveToThread(qThread);
        connect(stopButton, SIGNAL(clicked()), worker, SLOT(stop()), Qt::DirectConnection);
        connect(qThread, SIGNAL(started()), worker, SLOT(process()));
        connect(worker, SIGNAL(finished()), qThread, SLOT(quit()));
        connect(worker, SIGNAL(finished()), this, SLOT(enableAll()));
        connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
        connect(qThread, SIGNAL (finished()), qThread, SLOT (deleteLater()));

        qThread->start();
    }
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(errorDetails);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
}

void GameFrame::loadLock() {
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Choose locked cells file"),
                                                     "",
                                                     tr("JSON files (*.json)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file selected");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    lockedCellsLine->setText(file_name);
}

void GameFrame::lockCells() {
    QString errorDetails;
    QString path = lockedCellsLine->text();
    if (path.isEmpty()){
        errorDetails.append("- The file including locked cells is missing.\n");
    }

    if (errorDetails.isEmpty()){
        ifstream reader(path.toStdString());
        if (not reader.is_open())
            throw invalid_argument("Cannot open the file" + path.toStdString());

        nlohmann::json json;
        reader >> json;
        vector<int> toLock = json["lock"];

        for (int k : toLock){
            tilingFrame->getT()->setColor(k, "#9f4f08");
        }
        tilingFrame->repaint();
        reader.close();
    }
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(errorDetails);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
}

void GameFrame::zoomIn() {
    if (zoom_level < 300){
        zoom_level += 2;
        tilingFrame->setDilatation(zoom_level);
        tilingFrame->repaint();
    }
}

void GameFrame::zoomOut() {
    if (zoom_level > 2){
        zoom_level -= 2;
        tilingFrame->setDilatation(zoom_level);
        tilingFrame->repaint();
    }
}

void GameFrame::moveUp() {
    y_pos -= 5;
    tilingFrame->setYtranslation(y_pos);
    tilingFrame->repaint();
}

void GameFrame::moveDown() {
    y_pos += 5;
    tilingFrame->setYtranslation(y_pos);
    tilingFrame->repaint();
}

void GameFrame::moveLeft() {
    x_pos -= 5;
    tilingFrame->setXtranslation(x_pos);
    tilingFrame->repaint();
}

void GameFrame::moveRight() {
    x_pos += 5;
    tilingFrame->setXtranslation(x_pos);
    tilingFrame->repaint();
}

std::vector<std::shared_ptr<Player>> GameFrame::init2DPlayers(uint const &rb, uint const &rg) {
    QString errorDetails;
    vector<shared_ptr<Player>> players;

    if (porosityCheck->isChecked()){
        shared_ptr<PorosityPlayer> porosity = make_shared<PorosityPlayer>(rb, rg);
        players.push_back(porosity);
    }
    if (numBuildingCheck->isChecked()){
        QString minBStr = minBuildingLine->text();
        if (minBStr.isEmpty())
            errorDetails.append("- Min bound for buildings number is not set.\n");
        QString maxBStr = maxBuildingLine->text();
        if (maxBStr.isEmpty())
            errorDetails.append("- Max bound for buildings number is not set.\n");
        unsigned int minB = minBStr.toUInt();
        unsigned int maxB = maxBStr.toUInt();
        shared_ptr<NumBuildingPlayer> nb = make_shared<NumBuildingPlayer>(minB, maxB, rb, rg);
        players.push_back(nb);
    }
    if (buildingsPerimeterCheck->isChecked()){
        QString minPerimBStr = minPerimLine->text();
        if (minPerimBStr.isEmpty())
            errorDetails.append("- Min bound for buildings perimeter is not set.\n");
        QString maxPerimBStr = maxPerimLine->text();
        if (maxPerimBStr.isEmpty())
            errorDetails.append("- Max bound for buildings perimeter is not set.\n");
        double minPerimB = minPerimBStr.toDouble();
        double maxPerimB = maxPerimBStr.toDouble();
        shared_ptr<BuildingsPerimeterPlayer> perim = make_shared<BuildingsPerimeterPlayer>(minPerimB, maxPerimB, rb, rg);
        players.push_back(perim);
    }
    if (numGreenSpacesCheck->isChecked()){
        QString minGSStr = minGreenSpacesLine->text();
        if (minGSStr.isEmpty())
            errorDetails.append("- Min bound for green spaces number is not set.\n");
        QString maxGSStr = maxGreenSpacesLine->text();
        if (maxGSStr.isEmpty())
            errorDetails.append("- Max bound for green spaces number is not set.\n");
        unsigned int minGS = minGSStr.toUInt();
        unsigned int maxGS = maxGSStr.toUInt();
        shared_ptr<NumGreenSpacesPlayer> ng = make_shared<NumGreenSpacesPlayer>(minGS, maxGS, rb, rg);
        players.push_back(ng);
    }
    if (greenSpacesRateCheck->isChecked()){
        QString gsRateStr = greenSpaceRateLine->text();
        if (gsRateStr.isEmpty())
            errorDetails.append("- Rate for green space area is not set.\n");
        double gsRate = gsRateStr.toDouble();
        shared_ptr<GreenSpaceRatePlayer> gr = make_shared<GreenSpaceRatePlayer>(gsRate, rb, rg);
        players.push_back(gr);
    }
    if (errorDetails.isEmpty())
        return players;
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(errorDetails);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
        return {};
    }
}

std::vector<std::shared_ptr<Player>> GameFrame::init3DPlayers(const uint &rb, const uint &rg) {
    QString errorDetails;
    vector<shared_ptr<Player>> players;

    QString angleStr = angleLine->text();
    if (angleStr.isEmpty())
        errorDetails.append("- Projection angle is not set. \n");
    double angle_projection = angleStr.toDouble();

    QString hpStr = heightProjectionLine->text();
    if (hpStr.isEmpty())
        errorDetails.append("-  Projection height is not set. \n");
    double height_projection = hpStr.toDouble();

    if (porosityCheck->isChecked()){
        shared_ptr<PorosityPlayer> porosity = make_shared<PorosityPlayer>(rb, rg);
        players.push_back(porosity);
    }
    if (numBuildingCheck->isChecked()){
        QString minBStr = minBuildingLine->text();
        if (minBStr.isEmpty())
            errorDetails.append("- Min bound for buildings number is not set.\n");
        QString maxBStr = maxBuildingLine->text();
        if (maxBStr.isEmpty())
            errorDetails.append("- Max bound for buildings number is not set.\n");
        unsigned int minB = minBStr.toUInt();
        unsigned int maxB = maxBStr.toUInt();
        shared_ptr<NumBuilding3DPlayer> nb = make_shared<NumBuilding3DPlayer>(minB, maxB, rb, rg, angle_projection, height_projection);
        players.push_back(nb);
    }
    if (buildingsPerimeterCheck->isChecked()){
        QString minPerimBStr = minPerimLine->text();
        if (minPerimBStr.isEmpty())
            errorDetails.append("- Min bound for buildings perimeter is not set.\n");
        QString maxPerimBStr = maxPerimLine->text();
        if (maxPerimBStr.isEmpty())
            errorDetails.append("- Max bound for buildings perimeter is not set.\n");
        double minPerimB = minPerimBStr.toDouble();
        double maxPerimB = maxPerimBStr.toDouble();
        shared_ptr<Buildings3DPerimeterPlayer> perim = make_shared<Buildings3DPerimeterPlayer>(minPerimB, maxPerimB, rb, rg, angle_projection, height_projection);
        players.push_back(perim);
    }
    QString floorStr = floorHeightLine->text();
    if (floorStr.isEmpty())
        errorDetails.append("- Floor height is not set.\n");
    double floorHeight = floorStr.toDouble();
    shared_ptr<HeightPlayer> hp = make_shared<HeightPlayer>(floorHeight, angle_projection, height_projection);
    players.push_back(hp);

    if (numGreenSpacesCheck->isChecked()){
        QString minGSStr = minGreenSpacesLine->text();
        if (minGSStr.isEmpty())
            errorDetails.append("- Min bound for green spaces number is not set.\n");
        QString maxGSStr = maxGreenSpacesLine->text();
        if (maxGSStr.isEmpty())
            errorDetails.append("- Max bound for green spaces number is not set.\n");
        unsigned int minGS = minGSStr.toUInt();
        unsigned int maxGS = maxGSStr.toUInt();
        shared_ptr<NumGreenSpacesPlayer> ng = make_shared<NumGreenSpacesPlayer>(minGS, maxGS, rb, rg);
        players.push_back(ng);
    }
    if (greenSpacesRateCheck->isChecked()){
        QString gsRateStr = greenSpaceRateLine->text();
        if (gsRateStr.isEmpty())
            errorDetails.append("- Rate for green space area is not set.\n");
        double gsRate = gsRateStr.toDouble();
        shared_ptr<GreenSpaceRatePlayer> gr = make_shared<GreenSpaceRatePlayer>(gsRate, rb, rg);
        players.push_back(gr);
    }
    if (errorDetails.isEmpty())
        return players;
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(errorDetails);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
        return {};
    }
}

void GameFrame::save3DConfig() {
    shared_ptr<UrbanBlockGame> ubg = dynamic_pointer_cast<UrbanBlockGame>(gameplay->getGame());
    auto invariants = ubg->getInvariants();
    shared_ptr<Building3DInvariant> binv = dynamic_pointer_cast<Building3DInvariant>(invariants['b']);
    shared_ptr<GreenSpaces3DInvariant> ginv = dynamic_pointer_cast<GreenSpaces3DInvariant>(invariants['g']);

    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Save 3D configuartion"),
                                                     "",
                                                     tr("JSON files (*.json);; All files (*)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file given");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }else{
        int i = 0;
        for (auto const& [k, b] : binv->getComponents()){
            shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
            b3d->write(file_name.toStdString(), i == 0);
            i++;
        }

        for (auto const& [k, g] : ginv->getComponents()){
            shared_ptr<GroupOfCells3D> g3d = dynamic_pointer_cast<GroupOfCells3D>(g);
            g3d->write(file_name.toStdString());
        }
        QMessageBox messageBox;
        QString qs("Graph save in ");
        messageBox.setText(qs + file_name);
        messageBox.setIcon(QMessageBox::Information);
        messageBox.exec();
    }

}

void GameFrame::closeEvent(QCloseEvent *bar) {
    emit closed();
    bar->accept();
}


