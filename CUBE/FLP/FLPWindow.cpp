#include "FLPWindow.h"
#include "GeneticFLP.h"
#include "GeneticGUIListener.h"
#include "GeneticWorker.h"
#include "GeneticDisplay.h"
#include "FLPColors.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QStatusBar>
#include <QString>
#include <QPainter>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <utility>
#include <set>

using namespace std;

FLPWindow::FLPWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("FLP");
    setMinimumSize(1000, 1000);

    settings = new FLPSettingsWindow;

    auto* centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: white");

    graphFrame = new GraphFrame(graph, x_pos, y_pos, zoom_level, zoom_level/3., centralWidget);

    openAct = new QAction(tr("&Load graph"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file containing a graph"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save graph"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip("Save the current graph as a JSON file");
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    loadCostAct = new QAction(tr("Load costs"), this);
    loadCostAct->setStatusTip("Load costs from JSON file");
    connect(loadCostAct, SIGNAL(triggered()), this, SLOT(loadCosts()));

    saveCostAct = new QAction(tr("&Save cost"), this);
    saveCostAct->setStatusTip("Save the facilities and transport as a JSON file");
    connect(saveCostAct, SIGNAL(triggered()), this, SLOT(saveCosts()));

    refreshAct = new QAction(tr("&Refresh"), this);
    refreshAct->setShortcuts({Qt::Key_F5, QKeySequence::Refresh});
    connect(refreshAct, SIGNAL(triggered()), this, SLOT(refresh()));


    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(loadCostAct);
    fileMenu->addAction(saveCostAct);
    fileMenu->addAction(refreshAct);

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

    startAct = new QAction(tr("&Start"), this);
    startAct->setShortcut(Qt::Key_F1);
    setAct = new QAction(tr("&Settings"), this);
    computeFitnessAct = new QAction(tr("&Compute fitness"), this);
    searchMenu = menuBar()->addMenu(tr("&Search"));
    searchMenu->addAction(startAct);
    searchMenu->addAction(setAct);
    searchMenu->addAction(computeFitnessAct);
    connect(setAct, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(startAct, SIGNAL(triggered()), this, SLOT(startSearch()));
    connect(computeFitnessAct, SIGNAL(triggered()), this, SLOT(computeFitness()));

    slowAct = new QAction(tr("&Slow mode"), this);
    slowAct->setCheckable(true);
    slowAct->setStatusTip("Wait 200 ms between each step of the search");
    addCustomersAct = new QAction(tr("Add homes"), this);
    addCustomersAct->setCheckable(true);
    addFacilitiesAct = new QAction(tr("Add schools"), this);
    addFacilitiesAct->setCheckable(true);
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(slowAct);
    settingsMenu->addAction(addCustomersAct);
    settingsMenu->addAction(addFacilitiesAct);
    connect(addCustomersAct, SIGNAL(triggered()), this, SLOT(addCustomers()));
    connect(addFacilitiesAct, SIGNAL(triggered()), this, SLOT(addFacilities()));

    progressLabel = new QLabel("Progress: ", this);
    progressBar = new QProgressBar(this);
    progressBar->setValue(0);
    fitnessLabel = new QLabel(QString(": %1").arg("\u221E", -10), this);
    auto* red = new Circle(facilityColor, this);
    numRedLabel = new QLabel(QString(": %1").arg(" ", -10), this);
    auto* white = new Circle(customerColor, this);
    numWhiteLabel = new QLabel(QString(": %1").arg(" ", -10), this);
    costLoadedLabel = new QLabel("No cost loaded", this);
    costLoadedLabel->setStyleSheet("QLabel { color : darkRed; }");
    statusBar()->addPermanentWidget(progressLabel);
    statusBar()->addPermanentWidget(progressBar, 3);
    statusBar()->addPermanentWidget(fitnessLabel, 1);
    statusBar()->addPermanentWidget(white);
    statusBar()->addPermanentWidget(numWhiteLabel);
    statusBar()->addPermanentWidget(red);
    statusBar()->addPermanentWidget(numRedLabel, 2);
    statusBar()->addPermanentWidget(costLoadedLabel);

}

void FLPWindow::open() {
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Choose graph file"),
                                                     "",
                                                     tr("JSON files (*.json)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file selected");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    else{
        QFile file(file_name);
        if (not file.open(QIODevice::ReadOnly)){
            QMessageBox messageBox;
            messageBox.setText("Cannot open this file");
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.exec();
        }
        else{
            progressBar->setValue(0);
            fitnessLabel->setText("Fitness: \u221E");
            graph = make_shared<CoordGraph>();
            graph->loadJson(file_name.toStdString(), "weight", "data");
            graphFile = file_name.toStdString();
            graphFrame->setGraph(graph);
            graphFrame->repaint();
        }
    }
}

void FLPWindow::save() {
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Save graph"),
                                                     "",
                                                     tr("JSON files (*.json);; All files (*)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file given");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    else{
        if (not graph){
            QMessageBox messageBox;
            messageBox.setText("Graph not initialized");
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.exec();
        }
        else {
            graph->writeJson(file_name.toStdString());
            QMessageBox messageBox;
            QString qs("Graph save in ");
            messageBox.setText(qs + file_name);
            messageBox.setIcon(QMessageBox::Information);
            messageBox.exec();
        }
    }
}

void FLPWindow::zoomIn() {
    if (zoom_level < 300){
        zoom_level += 3;
        graphFrame->setDilatation(zoom_level);
        graphFrame->setRadius(zoom_level / 3.);
        graphFrame->repaint();
    }
}

void FLPWindow::zoomOut() {
    if (zoom_level > 3){
        zoom_level -= 3;
        graphFrame->setDilatation(zoom_level);
        graphFrame->setRadius(zoom_level / 3.);
        graphFrame->repaint();
    }
}

void FLPWindow::showSettings() {
    settings->show();
}

void FLPWindow::startSearch() {
    if (not graph){
        QMessageBox messageBox;
        messageBox.setText("No graph loaded");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    } else if (transportCost.empty() and not settings->useDefaultTransportCost()){
        QMessageBox messageBox;
        messageBox.setText("No transport cost loaded nor default value used.");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    } else if (facilitiesCost.empty() and not settings->useDefaultFacilitiesCost()){
        QMessageBox messageBox;
        messageBox.setText("No facilities cost loaded nor default value used.");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    else{
        vector<uint64_t> white_nodes;
        shared_ptr<set<int>> forced0 = make_shared<set<int>>();
        shared_ptr<set<int>> forced1 = make_shared<set<int>>();
        for (int i = 0; i < graph->size(); ++i) {
            if (graph->getColor((*graph)[i]) != "#acacac")
                white_nodes.push_back((*graph)[i]);
            if (graph->getColor((*graph)[i]) == forcedCustomerColor)
                forced0->insert(i);
            if (graph->getColor((*graph)[i]) == forcedFacilityColor)
                forced1->insert(i);
        }

        if(settings->useDefaultTransportCost()) {
            for (uint64_t const &node: white_nodes) {
                auto d = graph->dijkstra(node, ProblemType::shortest_path);
                transportCost.push_back(d);
            }
        }

        FLPSettings flpSettings = settings->getSettings();

        if (settings->useDefaultFacilitiesCost())
            facilitiesCost = vector<double>(white_nodes.size(), flpSettings.facilitiesCost);

        costLoadedLabel->setText("Cost loaded");
        costLoadedLabel->setStyleSheet("QLabel {color : darkGreen;}");

        shared_ptr<GeneticFLP> problem;
        if (not settings->useConstraint()) {
            problem = make_shared<GeneticFLP>(flpSettings.populationSize,
                                              flpSettings.numGeneration,
                                              white_nodes.size(),
                                              flpSettings.mutationRate,
                                              facilitiesCost,
                                              transportCost);
        } else{
            problem = make_shared<GeneticConstrainedFLP>(flpSettings.populationSize,
                                                         flpSettings.numGeneration,
                                                         white_nodes.size(),
                                                         flpSettings.mutationRate,
                                                         facilitiesCost,
                                                         transportCost,
                                                         flpSettings.maxDist,
                                                         flpSettings.penalty);
        }
        problem->setForced0(forced0);
        problem->setForced1(forced1);
        shared_ptr<GeneticGUIListener> l = make_shared<GeneticGUIListener>(graph, white_nodes, slowAct->isChecked());
        problem->add_listener(l);
        connect(l.get(), SIGNAL(doUpdate()), graphFrame, SLOT(repaint()));
        connect(l.get(), SIGNAL(generationChanged(int)), progressBar, SLOT(setValue(int)));
        connect(l.get(), SIGNAL(fitnessChanged(double)), this, SLOT(updateFitness(double)));
        connect(l.get(), SIGNAL(factoriesChanged(long,long)), this, SLOT(updateCircle(long,long)));

        auto* qThread = new QThread;
        auto* worker = new GeneticWorker(problem);
        worker->moveToThread(qThread);
        connect(qThread, SIGNAL(started()), worker, SLOT(process()));
        connect(worker, SIGNAL(finished()), qThread, SLOT(quit()));
        connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
        connect(qThread, SIGNAL (finished()), qThread, SLOT (deleteLater()));

        qThread->start();
    }
}

void FLPWindow::refresh() {
    graph = make_shared<CoordGraph>();
    graph->loadJson(graphFile, "weight", "data");
    graphFrame->setGraph(graph);
    graphFrame->repaint();
    progressBar->setValue(0);
    fitnessLabel->setText(QString(": %1").arg("\u221E", -10));
    numWhiteLabel->setText(QString(": %1").arg(" ", -10));
    numRedLabel->setText(QString(": %1").arg(" ", -10));
}

void FLPWindow::updateFitness(double x) {
    fitnessLabel->setText(QString(": %1").arg(x, -10));
}

void FLPWindow::saveCosts() {
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Save costs"),
                                                     "",
                                                     tr("JSON files (*.json);; All files (*)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file given");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    } else{
        nlohmann::json json;
        json["facilities"] = facilitiesCost;
        json["transport"] = transportCost;
        ofstream writer;
        writer.open(file_name.toStdString(), ios::out);
        writer << json.dump(4);
        writer.close();
    }
}

void FLPWindow::loadCosts() {
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Choose cost file"),
                                                     "",
                                                     tr("JSON files (*.json)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty()){
        QMessageBox messageBox;
        messageBox.setText("No file selected");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    else{
        QFile file(file_name);
        if (not file.open(QIODevice::ReadOnly)){
            QMessageBox messageBox;
            messageBox.setText("Cannot open this file");
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.exec();
        }
        else{
            nlohmann::json json;
            ifstream reader(file_name.toStdString());
            reader >> json;
            reader.close();
            try {
                transportCost = json["transport"].get<vector<vector<double>>>();
                facilitiesCost = json["facilities"].get<vector<double>>();
                costLoadedLabel->setText("Cost loaded");
                costLoadedLabel->setStyleSheet("QLabel {color : darkGreen; }");
            }catch (exception const& e){
                QMessageBox messageBox;
                messageBox.setText("Cannot load data from the selected file. The file must contain the key "
                                   "\"transport\" and the key \"facilities\"!");
                messageBox.setIcon(QMessageBox::Critical);
                messageBox.exec();
            }

        }
    }
}

void FLPWindow::computeFitness() {
    if (not graph){
        QMessageBox messageBox;
        messageBox.setText("No graph loaded");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    } else if (transportCost.empty() and not settings->useDefaultTransportCost()){
        QMessageBox messageBox;
        messageBox.setText("No transport cost loaded nor default value used.");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    } else if (facilitiesCost.empty() and not settings->useDefaultFacilitiesCost()){
        QMessageBox messageBox;
        messageBox.setText("No facilities cost loaded nor default value used.");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
    else {
        vector<uint64_t> solution_nodes;
        for (int i = 0; i < graph->size(); ++i) {
            if (graph->getColor((*graph)[i]) == customerColor
            or graph->getColor((*graph)[i]) == facilityColor
            or graph->getColor((*graph)[i]) == forcedCustomerColor
            or graph->getColor((*graph)[i]) == forcedFacilityColor)
                solution_nodes.push_back((*graph)[i]);
        }

        if (settings->useDefaultTransportCost()) {
            for (uint64_t const &node: solution_nodes) {
                auto d = graph->dijkstra(node, ProblemType::shortest_path);
                transportCost.push_back(d);
            }
        }

        FLPSettings flpSettings = settings->getSettings();

        if (settings->useDefaultFacilitiesCost())
            facilitiesCost = vector<double>(solution_nodes.size(), flpSettings.facilitiesCost);

        costLoadedLabel->setText("Cost loaded");
        costLoadedLabel->setStyleSheet("QLabel {color : darkGreen;}");

        shared_ptr<GeneticFLP> problem;
        if (not settings->useConstraint()) {
            problem = make_shared<GeneticFLP>(flpSettings.populationSize,
                                              flpSettings.numGeneration,
                                              solution_nodes.size(),
                                              flpSettings.mutationRate,
                                              facilitiesCost,
                                              transportCost);
        } else {
            problem = make_shared<GeneticConstrainedFLP>(flpSettings.populationSize,
                                                         flpSettings.numGeneration,
                                                         solution_nodes.size(),
                                                         flpSettings.mutationRate,
                                                         facilitiesCost,
                                                         transportCost,
                                                         flpSettings.maxDist,
                                                         flpSettings.penalty);
        }
        vector<bool> sol;
        for (uint64_t const& node : solution_nodes){
            if (graph->getColor(node) == facilityColor or graph->getColor(node) == forcedFacilityColor)
                sol.push_back(true);
            if(graph->getColor(node) == customerColor or graph->getColor(node) == forcedCustomerColor)
                sol.push_back(false);
        }
        shared_ptr<Genes> genes = make_shared<Genes>(sol);
        double x = problem->fitness(genes);
        fitnessLabel->setText("Fitness: " + QString::fromStdString(to_string(x)));
        numWhiteLabel->setText(QString(": %1").arg(genes->num0(), -10));
        numRedLabel->setText(QString(": %1").arg(genes->num1(), -10));
    }

}

void FLPWindow::updateCircle(long num_0, long num_1) {
    numWhiteLabel->setText(QString(": %1").arg(num_0, -10));
    numRedLabel->setText(QString(": %1").arg(num_1, -10));
}

void FLPWindow::moveUp() {
    y_pos -= 5;
    graphFrame->setYtranslation(y_pos);
    graphFrame->repaint();

}

void FLPWindow::moveDown() {
    y_pos += 5;
    graphFrame->setYtranslation(y_pos);
    graphFrame->repaint();
}

void FLPWindow::moveLeft() {
    x_pos -= 5;
    graphFrame->setXtranslation(x_pos);
    graphFrame->repaint();

}

void FLPWindow::moveRight() {
    x_pos += 5;
    graphFrame->setXtranslation(x_pos);
    graphFrame->repaint();
}

void FLPWindow::addCustomers() {
    if (not graph){
        addCustomersAct->setChecked(false);
        QMessageBox messageBox;
        messageBox.setText("No graph loaded. Cannot add customers");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();

    }else{
        graphFrame->setEditable(addCustomersAct->isChecked());
        //graphFrame->setEditColor("#fffdcd");
        graphFrame->setEditColor(forcedCustomerColor);
        addFacilitiesAct->setEnabled(not addCustomersAct->isChecked());
    }
}

void FLPWindow::addFacilities() {
    if (not graph){
        addCustomersAct->setChecked(false);
        QMessageBox messageBox;
        messageBox.setText("No graph loaded. Cannot add facilities");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();

    }else{
        graphFrame->setEditable(addFacilitiesAct->isChecked());
            graphFrame->setEditColor(forcedFacilityColor);
        addCustomersAct->setEnabled(not addFacilitiesAct->isChecked());
    }
}

void FLPWindow::closeEvent(QCloseEvent *bar) {
    emit closed();
    bar->accept();
}

Circle::Circle(std::string c, QWidget *parent): color(std::move(c)), QWidget(parent) {
    setMinimumSize(20, 20);
}

void Circle::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 1));
    painter.setRenderHint(QPainter::Antialiasing);
    QString qcolor = QString::fromStdString(color);
    painter.setBrush(QBrush(QColor(qcolor)));

    painter.drawEllipse(0, 0, 20, 20);


}


