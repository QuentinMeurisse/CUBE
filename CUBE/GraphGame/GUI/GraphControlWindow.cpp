#include "GraphControlWindow.h"
#include "../GraphGame.h"
#include "../Invariants/ColorNodeInvariant.h"
#include "../../DataStructures/GraphUtility.h"
#include "../Players/MovePlayer.h"
#include "IterativeBest.h"
#include "../Players/VariableMovePlayer.h"
#include "VariableIterativeBest.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <iostream>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <map>

using namespace std;

GraphControlWindow::GraphControlWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Graph");
    resize(1920, 1080);
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* playersGroup = new QGroupBox("Players", centralWidget);
    QStringList players_type = {"Not cooperative", "Cooperative", "KCenter"};
    playersTable = new QTableWidget(5, 2, playersGroup);
    auto* qcb = new QComboBox(playersTable);
    qcb->addItems(players_type);
    playersTable->setCellWidget(0, 0, qcb);
    playersTable->setItem(1, 0, new QTableWidgetItem("#00ffff"));
    playersTable->setItem(2, 0, new QTableWidgetItem("#00e1e1"));
    playersTable->setItem(3, 0, new QTableWidgetItem("#00c3c3"));
    playersTable->setItem(4, 0, new QTableWidgetItem("#00a5a5"));
    playersTable->setItem(1, 1, new QTableWidgetItem("#ffb300"));
    playersTable->setItem(2, 1, new QTableWidgetItem("#ff9500"));
    playersTable->setItem(3, 1, new QTableWidgetItem("#ff7700"));
    playersTable->setItem(4, 1, new QTableWidgetItem("#ff5900"));

    playersTable->setColumnWidth(0, 200);
    qcb = new QComboBox(playersTable);
    qcb->addItems(players_type);
    playersTable->setCellWidget(0, 1, qcb);
    playersTable->setColumnWidth(1, 200);
    playersTable->resize(400, 400);

    addRow = new QPushButton("+ row", playersGroup);
    addRow->setFixedWidth(150);
    removeRow = new QPushButton("- row", playersGroup);
    removeRow->setFixedWidth(150);
    addColumn = new QPushButton("+ column", playersGroup);
    addColumn->setFixedWidth(150);
    removeColumn = new QPushButton("- column", playersGroup);
    removeColumn->setFixedWidth(150);
    startButton = new QPushButton("Start", playersGroup);
    useOldTokens = new QCheckBox("Use old tokens", playersGroup);
    useMemory = new QCheckBox("Use memory", playersGroup);
    auto* maxMemorySizeLabel = new QLabel("Max memory size", playersGroup);
    maxMemoryLine = new QLineEdit(centralWidget);
    maxMemoryLine->setFixedWidth(50);
    maxMemoryLine->setEnabled(false);
    auto* maxDuplication = new QLabel("Max duplication", playersGroup);
    maxDuplicateLine = new QLineEdit(centralWidget);
    maxDuplicateLine->setFixedWidth(50);
    maxDuplicateLine->setEnabled(false);
    useVariableNeigh = new QCheckBox("Use variable neigh", playersGroup);
    variableLine = new QLineEdit(playersGroup);
    variableLine->setFixedWidth(50);
    variableLine->setEnabled(false);

    auto* playersLayout = new QGridLayout;
    playersLayout->addWidget(playersTable, 0, 0, 8, 3);
    playersLayout->addWidget(removeRow, 0, 4);
    playersLayout->addWidget(addRow, 0, 5);
    playersLayout->addWidget(removeColumn, 1, 4);
    playersLayout->addWidget(addColumn, 1, 5);
    playersLayout->addWidget(useOldTokens, 2, 4, 1, 2);
    playersLayout->addWidget(useMemory, 3, 4, 1, 2);
    playersLayout->addWidget(maxMemorySizeLabel, 4, 4);
    playersLayout->addWidget(maxMemoryLine, 4, 5);
    playersLayout->addWidget(maxDuplication, 5, 4);
    playersLayout->addWidget(maxDuplicateLine, 5, 5);
    playersLayout->addWidget(useVariableNeigh, 6, 4);
    playersLayout->addWidget(variableLine, 6, 5);
    playersLayout->addWidget(startButton, 7, 4, 1, 2);
    playersGroup->setLayout(playersLayout);
    playersGroup->move(10, 10);
    playersGroup->resize(830, 400);
    connect(startButton, SIGNAL(clicked()), this, SLOT(startAction()));
    connect(addRow, SIGNAL(clicked()), this, SLOT(addRowAction()));
    connect(removeRow, SIGNAL(clicked()), this, SLOT(removeRowAction()));
    connect(addColumn, SIGNAL(clicked()), this, SLOT(addColumnAction()));
    connect(removeColumn, SIGNAL(clicked()), this, SLOT(removeColumnAction()));
    connect(useOldTokens, SIGNAL(stateChanged(int)), this, SLOT(useOldTokensAction(int)));
    connect(useMemory, SIGNAL(stateChanged(int)), this, SLOT(useMemoryAction(int)));
    connect(useVariableNeigh, SIGNAL(stateChanged(int)), this, SLOT(useVariableAction(int)));

    auto* oldTokenGroup = new QGroupBox("Old tokens", centralWidget);
    QStringList token_type = {"Not players", "Players", "KCenter"};
    oldTokensTable = new QTableWidget(5, 2, oldTokenGroup);
    auto* token_qcb = new QComboBox(oldTokensTable);
    token_qcb->addItems(token_type);
    oldTokensTable->setCellWidget(0, 0, token_qcb);
    oldTokensTable->setColumnWidth(0, 200);
    token_qcb = new QComboBox(oldTokensTable);
    token_qcb->addItems(token_type);
    oldTokensTable->setCellWidget(0, 1, token_qcb);
    oldTokensTable->setItem(1, 0, new QTableWidgetItem("#00ffff"));
    oldTokensTable->setItem(2, 0, new QTableWidgetItem("#00e1e1"));
    oldTokensTable->setItem(3, 0, new QTableWidgetItem("#00c3c3"));
    oldTokensTable->setItem(4, 0, new QTableWidgetItem("#00a5a5"));
    oldTokensTable->setItem(1, 1, new QTableWidgetItem("#ffb300"));
    oldTokensTable->setItem(2, 1, new QTableWidgetItem("#ff9500"));
    oldTokensTable->setItem(3, 1, new QTableWidgetItem("#ff7700"));
    oldTokensTable->setItem(4, 1, new QTableWidgetItem("#ff5900"));

    oldTokensTable->setColumnWidth(1, 200);
    oldTokensTable->resize(400, 400);

    addRowToken = new QPushButton("+ row", oldTokenGroup);
    removeRowToken = new QPushButton("- row", oldTokenGroup);
    addColumnToken = new QPushButton("+ column", oldTokenGroup);
    removeColumnToken = new QPushButton("- column", oldTokenGroup);
    auto* oldTokenLayout = new QGridLayout;
    oldTokenLayout->addWidget(oldTokensTable, 0, 0, 4, 3);
    oldTokenLayout->addWidget(removeRowToken, 0, 4);
    oldTokenLayout->addWidget(addRowToken, 0, 5);
    oldTokenLayout->addWidget(removeColumnToken, 1, 4);
    oldTokenLayout->addWidget(addColumnToken, 1, 5);
    oldTokenGroup->setLayout(oldTokenLayout);
    oldTokenGroup->move(1000, 10);
    oldTokenGroup->resize(750, 400);
    oldTokensTable->setEnabled(false);
    addRowToken->setEnabled(false);
    removeRowToken->setEnabled(false);
    addColumnToken->setEnabled(false);
    removeColumnToken->setEnabled(false);
    connect(addRowToken, SIGNAL(clicked()), this, SLOT(addRowTokeAction()));
    connect(removeRowToken, SIGNAL(clicked()), this, SLOT(removeRowTokenAction()));
    connect(addColumnToken, SIGNAL(clicked()), this, SLOT(addColumnTokenAction()));
    connect(removeColumnToken, SIGNAL(clicked()), this, SLOT(removeColumnTokenAction()));

    auto* importExportGroup = new QGroupBox("Import / Export", centralWidget);
    importExportGroup->resize(300, 200);
    loadGraphButton = new QPushButton("Load", centralWidget);
    loadGraphButton->setToolTip(tr("Load a graph from a json file"));
    loadGraphButton->resize(50, 200);
    auto* importExportLayout = new QVBoxLayout;
    saveGraphButton = new QPushButton("Save", centralWidget);
    importExportLayout->addWidget(loadGraphButton);
    importExportLayout->addWidget(saveGraphButton);
    importExportGroup->setLayout(importExportLayout);
    importExportGroup->move(10, 450);
    connect(loadGraphButton, SIGNAL(clicked()), this, SLOT(loadGraphAction()));
    connect(saveGraphButton, SIGNAL(clicked()), this, SLOT(saveGraphAction()));

}

void GraphControlWindow::startAction() {
    if (not g){
        QMessageBox messageBox;
        messageBox.setText("Graph not loaded");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
        return;
    }

    shared_ptr<GraphGame> gg = make_shared<GraphGame>(g);
    vector<shared_ptr<Player>> players;

    function<bool (double, double)> betterOrEqual = [&](double x, double y){
        return x <= y;
    };

    QString max_memory_size = maxMemoryLine->text();
    QString max_duplication = maxDuplicateLine->text();
    QString use_variable = variableLine->text();
    if (useOldTokens->isChecked()){
        map<string, uint64_t> colored_nodes;
        for (int k = 0; k < g->size(); ++k) {
            uint64_t node_id = (*g)[k];
            string c = g->getColor(node_id);
            if (c != "#ffffff" and c != "#dadada")
                colored_nodes[c] = node_id;
        }
        for (int j = 0; j < oldTokensTable->columnCount(); ++j) {
            char key = static_cast<char>('0' + j);
            shared_ptr<ColorNodeInvariant> cni = make_shared<ColorNodeInvariant>(gg);
            gg->addInvariant(cni, key);
            for (int i = 1; i < oldTokensTable->rowCount(); ++i) {
                auto *item = oldTokensTable->item(i, j);
                string color = item ? item->text().toStdString() : "";
                if (not color.empty()){
                    cni->addNode(colored_nodes[color]);
                    auto* is_player = (QComboBox*) oldTokensTable->cellWidget(0, j);
                    if (is_player->currentText() == "Players") {
                        auto *player_mode = (QComboBox *) playersTable->cellWidget(0, j);
                        if (useVariableNeigh->isChecked()) {
                            if (player_mode->currentText() == "Not cooperative") {
                                shared_ptr<VariableMovePlayer> p = make_shared<VariableMovePlayer>(use_variable.toInt(),
                                                                                                   0,
                                                                                                   key,
                                                                                                   cni->numTokens() - 1,
                                                                                                   betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);
                            } else if (player_mode->currentText() == "Cooperative"){
                                shared_ptr<VariableCooperativeMovePlayer> p = make_shared<VariableCooperativeMovePlayer>(use_variable.toInt(),
                                                                                                   0,
                                                                                                   key,
                                                                                                   cni->numTokens() - 1,
                                                                                                   betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);
                            } else{
                                shared_ptr<VariableKCenterPlayer> p = make_shared<VariableKCenterPlayer>(use_variable.toInt(),
                                                                                                         key,
                                                                                                         cni->numTokens() - 1,
                                                                                                         betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);
                            }
                        }
                        else {
                            if (player_mode->currentText() == "Not cooperative") {
                                shared_ptr<MovePlayer> p = make_shared<MovePlayer>(0, key, cni->numTokens() - 1,
                                                                                   betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);
                            } else if (player_mode->currentText() == "Cooperative"){
                                shared_ptr<CooperativeMovePlayer> p = make_shared<CooperativeMovePlayer>(0, key,
                                                                                                         cni->numTokens() -
                                                                                                         1,
                                                                                                         betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);
                            } else {
                                shared_ptr<KCenterPlayer> p = make_shared<KCenterPlayer>(key,
                                                                                         cni->numTokens() - 1,
                                                                                         betterOrEqual);
                                if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                                    p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                                players.push_back(p);

                            }
                        }
                    }
                }
            }
        }
    }

    for (int j = 0; j < playersTable->columnCount(); ++j) {
        char key = static_cast<char>('0' + j);
        shared_ptr<ColorNodeInvariant> cni;
        if (useOldTokens->isChecked() and j < oldTokensTable->columnCount()){
            auto inv = gg->getInvariants();
            cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[key]);
        }
        else {
            cni = make_shared<ColorNodeInvariant>(gg);
            gg->addInvariant(cni, key);
        }
        bool all_empty = true;
        for (int i = 1; i < playersTable->rowCount(); ++i) {
            auto *item = playersTable->item(i, j);
            string color = item ? item->text().toStdString() : "";
            if (not color.empty()) {
                all_empty = false;
                GraphUtility::randomColor(gg, color, key);
                auto *player_mode = (QComboBox *) playersTable->cellWidget(0, j);
                if (useVariableNeigh->isChecked()) {
                    if (player_mode->currentText() == "Not cooperative") {
                        shared_ptr<VariableMovePlayer> p = make_shared<VariableMovePlayer>(use_variable.toInt(),
                                                                                           0,
                                                                                           key,
                                                                                           cni->numTokens() - 1,
                                                                                           betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    } else if (player_mode->currentText() == "Cooperative"){
                        shared_ptr<VariableCooperativeMovePlayer> p = make_shared<VariableCooperativeMovePlayer>(use_variable.toInt(),
                                                                                                 0,
                                                                                                 key,
                                                                                                 cni->numTokens() - 1,
                                                                                                 betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    } else{
                        shared_ptr<VariableKCenterPlayer> p = make_shared<VariableKCenterPlayer>(use_variable.toInt(),
                                                                                                 key,
                                                                                                 cni->numTokens() - 1,
                                                                                                 betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    }
                }
                else {
                    if (player_mode->currentText() == "Not cooperative") {
                        shared_ptr<MovePlayer> p = make_shared<MovePlayer>(0, key, cni->numTokens() - 1, betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    } else if (player_mode->currentText() == "Cooperative"){
                        shared_ptr<CooperativeMovePlayer> p = make_shared<CooperativeMovePlayer>(0, key,
                                                                                                 cni->numTokens() - 1,
                                                                                                 betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    } else{
                        shared_ptr<KCenterPlayer> p = make_shared<KCenterPlayer>(key,
                                                                                 cni->numTokens() - 1,
                                                                                 betterOrEqual);
                        if (not max_memory_size.isEmpty() and not max_duplication.isEmpty())
                            p->setMemory(max_memory_size.toInt(), max_duplication.toInt());
                        players.push_back(p);
                    }
                }
            }
        }
        if (all_empty and useOldTokens->isChecked() and j >= oldTokensTable->columnCount())
            gg->removeInvariant(key);

    }
    if (players.empty()){
        QMessageBox messageBox;
        messageBox.setText("No players given");
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        IterativeBest gameplay(players, gg);
        shared_ptr<IterativeBest> gameplay_ptr = make_shared<IterativeBest>(gameplay);
        gw = new GraphWindow(g, gameplay_ptr);
        gw->disableStartButton();
        gw->disableStopButton();
        gw->show();
        return;
    }else{
        shared_ptr<IterativeGameplay> gameplay_ptr;
        if (useMemory->isChecked() and useVariableNeigh->isChecked())
            gameplay_ptr = make_shared<VariableIterativeBestWithMemory>(players, gg);
        else if(not useMemory->isChecked() and useVariableNeigh->isChecked())
            gameplay_ptr = make_shared<VariableIterativeBest>(players, gg);
        else if(useMemory->isChecked() and not useVariableNeigh->isChecked())
            gameplay_ptr = make_shared<IterativeBestWithMemory>(players, gg);
        else
            gameplay_ptr = make_shared<IterativeBest>(players, gg);
        gw = new GraphWindow(g, gameplay_ptr);
        gw->show();
    }

}

void GraphControlWindow::addRowAction() {
    int num = playersTable->rowCount();
    playersTable->setRowCount(num + 1);
}

void GraphControlWindow::removeRowAction() {
    int num = playersTable->rowCount();
    if (num > 2)
        playersTable->setRowCount(num - 1);
}

void GraphControlWindow::addColumnAction() {
    int num = playersTable->columnCount();
    playersTable->setColumnCount(num + 1);
    QStringList players_type = {"Not cooperative", "Cooperative", "KCenter"};
    auto* qcb = new QComboBox(playersTable);
    qcb->addItems(players_type);
    playersTable->setCellWidget(0, num, qcb);
    playersTable->setColumnWidth(num, 200);
}

void GraphControlWindow::removeColumnAction() {
    int num = playersTable->columnCount();
    if (num > 1)
        playersTable->setColumnCount(num - 1);
}

void GraphControlWindow::loadGraphAction() {
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
            g = make_shared<CoordGraph>();
            g->loadJson(file_name.toStdString(), "weight", "data");
        }
    }
}

void GraphControlWindow::saveGraphAction() {
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Save graphe"),
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
        if (not g){
            QMessageBox messageBox;
            messageBox.setText("Graph not initialized");
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.exec();
        }
        else {
            g->writeJson(file_name.toStdString());
            QMessageBox messageBox;
            QString qs("Graph save in ");
            qs + file_name;
            messageBox.setText(qs + file_name);
            messageBox.setIcon(QMessageBox::Information);
            messageBox.exec();
        }
    }
}

void GraphControlWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete or event->key() == Qt::Key_Backspace){
        auto* current_item = playersTable->currentItem();
        if (current_item and current_item->row() > 0){
            playersTable->takeItem(current_item->row(), current_item->column());
        }
    }
}

void GraphControlWindow::addRowTokeAction() {
    int num = oldTokensTable->rowCount();
    oldTokensTable->setRowCount(num + 1);
}

void GraphControlWindow::removeRowTokenAction() {
    int num = oldTokensTable->rowCount();
    if (num > 2)
        oldTokensTable->setRowCount(num - 1);
}

void GraphControlWindow::addColumnTokenAction() {
    int num = oldTokensTable->columnCount();
    oldTokensTable->setColumnCount(num + 1);
    QStringList players_type = {"Not players", "Players", "KCenter"};
    auto* qcb = new QComboBox(oldTokensTable);
    qcb->addItems(players_type);
    oldTokensTable->setCellWidget(0, num, qcb);
    oldTokensTable->setColumnWidth(num, 200);
}

void GraphControlWindow::removeColumnTokenAction() {
    int num = oldTokensTable->columnCount();
    if (num > 1)
        oldTokensTable->setColumnCount(num - 1);
}

void GraphControlWindow::useOldTokensAction(int state) {
    oldTokensTable->setEnabled(state == Qt::Checked);
    addRowToken->setEnabled(state == Qt::Checked);
    removeRowToken->setEnabled(state == Qt::Checked);
    addColumnToken->setEnabled(state == Qt::Checked);
    removeColumnToken->setEnabled(state == Qt::Checked);
}

void GraphControlWindow::useMemoryAction(int state) {
    maxMemoryLine->setEnabled(state == Qt::Checked);
    maxDuplicateLine->setEnabled(state == Qt::Checked);
}

void GraphControlWindow::useVariableAction(int state) {
    variableLine->setEnabled(state == Qt::Checked);
}

void GraphControlWindow::closeEvent(QCloseEvent *bar) {
    emit closed();
    bar->accept();
}


