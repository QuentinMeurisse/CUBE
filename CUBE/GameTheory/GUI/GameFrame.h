#ifndef CUBE_GAMEFRAME_H
#define CUBE_GAMEFRAME_H

#include "../Tiling.h"
#include "TilingFrame.h"
#include "IterativeGameplay.h"
#include "../UrbanBlockGame.h"


#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QThread>
#include <QSlider>
#include <QStackedWidget>
#include <memory>
#include <QtWidgets/QLabel>
#include <QMenu>
#include <QAction>
class GameFrame : public QMainWindow{
    Q_OBJECT
public:
    explicit GameFrame(QWidget *parent = nullptr);

private:
    void initGameplay(std::shared_ptr<UrbanBlockGame> const& game);
    std::vector<std::shared_ptr<Player>> init2DPlayers(uint const &rb, uint const &rg);
    std::vector<std::shared_ptr<Player>> init3DPlayers(uint const &rb, uint const &rg);


private slots:
    void initTiling();
    void updateNumBuildings(int state);
    void updateBuildingPerim(int state);
    void updateGreenSpacesrate(int state);
    void updateNumGreenSpaces(int state);
    void active3DMode(int state);
    void initGame();

    void enableAll();
    void resetSolution();
    void restartGameplay();
    void showMaxTimeField(bool state);
    void loadLock();
    void lockCells();

    void save3DConfig();

    void zoomIn();
    void zoomOut();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

signals:
    void finished();


private:
    TilingFrame* tilingFrame;
    QRadioButton* deterministGameplay;
    QRadioButton* randomGameplay;
    QCheckBox* startFromRandom;
    QCheckBox* porosityCheck;
    QCheckBox* numBuildingCheck;
    QLineEdit* minBuildingLine;
    QLineEdit* maxBuildingLine;
    QCheckBox* buildingsPerimeterCheck;
    QLineEdit* minPerimLine;
    QLineEdit* maxPerimLine;
    QCheckBox* greenSpacesRateCheck;
    QLineEdit* greenSpaceRateLine;
    QCheckBox* numGreenSpacesCheck;
    QLineEdit* minGreenSpacesLine;
    QLineEdit* maxGreenSpacesLine;
    QLineEdit* tilingW;
    QLineEdit* tilingH;
    QLineEdit* tilingA;
    QPushButton* setupTilingButton;
    QRadioButton* squareTiling;
    QRadioButton* hexagonTiling;
    QRadioButton* triangleTiling;
    QPushButton* resetSolutionButton;
    QStackedWidget* startAndRestartButtons;
    QWidget* maxTimeWidget;
    QLineEdit* maxTimeLine;
    QLineEdit* rbLine;
    QLineEdit* rgLine;
    QPushButton* stopButton;
    QPushButton* loadLockedCellsButton;
    QLineEdit* lockedCellsLine;
    QPushButton* lockCellsButton;

    QMenu* zoomMenu;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* moveUpAct;
    QAction* moveDownAct;
    QAction* moveLeftAct;
    QAction* moveRightAct;

    QCheckBox* use3D;
    QLineEdit* floorHeightLine;
    QLineEdit* heightProjectionLine;
    QLineEdit* angleLine;
    QPushButton* save3DButton;


    std::shared_ptr<IterativeGameplay> gameplay;
    int x_pos = 0;
    int y_pos = 0;
    int zoom_level = 10;



};



#endif //CUBE_GAMEFRAME_H
