#ifndef CUBE_FLPWINDOW_H
#define CUBE_FLPWINDOW_H

#include "GUI/GraphFrame.h"
#include "graph.h"
#include "FLPSettingsWindow.h"

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QProgressBar>
#include <QLabel>
#include <memory>
#include <string>


class FLPWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit FLPWindow(QWidget* parent = nullptr);

protected slots:
    void open();
    void save();
    void zoomIn();
    void zoomOut();
    void showSettings();
    void startSearch();
    void refresh();
    void updateFitness(double x);
    void saveCosts();
    void loadCosts();
    void computeFitness();
    void updateCircle(long num_0, long num_1);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void addCustomers();
    void addFacilities();

private:
    GraphFrame* graphFrame;

    QMenu* fileMenu;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveCostAct;
    QAction* loadCostAct;
    QAction* refreshAct;

    QMenu* zoomMenu;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* moveUpAct;
    QAction* moveDownAct;
    QAction* moveLeftAct;
    QAction* moveRightAct;


    FLPSettingsWindow* settings;

    QMenu* searchMenu;
    QAction* startAct;
    QAction* setAct;
    QAction* computeFitnessAct;

    QMenu* settingsMenu;
    QAction* slowAct;
    QAction* addCustomersAct;
    QAction* addFacilitiesAct;

    QProgressBar* progressBar;
    QLabel* progressLabel;
    QLabel* fitnessLabel;
    QLabel* numWhiteLabel;
    QLabel* numRedLabel;
    QLabel* costLoadedLabel;

    std::shared_ptr<CoordGraph> graph = std::make_shared<CoordGraph>();
    int x_pos = 15;
    int y_pos = 15;
    int zoom_level = 30;
    std::string graphFile;
    std::vector<double> facilitiesCost;
    std::vector<std::vector<double>> transportCost;


};

class Circle : public QWidget{
    Q_OBJECT
public:
    explicit Circle (std::string c, QWidget* parent= nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::string color;
};


#endif //CUBE_FLPWINDOW_H
