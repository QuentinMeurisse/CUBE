#ifndef CUBE_GRAPHCONTROLWINDOW_H
#define CUBE_GRAPHCONTROLWINDOW_H

#include "../graph.h"
#include "GraphWindow.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <memory>
#include <QKeyEvent>
#include <QCheckBox>


class GraphControlWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit GraphControlWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void startAction();
    void addRowAction();
    void removeRowAction();
    void addColumnAction();
    void removeColumnAction();
    void loadGraphAction();
    void saveGraphAction();
    void addRowTokeAction();
    void removeRowTokenAction();
    void addColumnTokenAction();
    void removeColumnTokenAction();
    void useOldTokensAction(int state);
    void useMemoryAction(int state);
    void useVariableAction(int state);

private:
    QTableWidget* playersTable;
    QPushButton* startButton;
    QPushButton* addRow;
    QPushButton* removeRow;
    QPushButton* addColumn;
    QPushButton* removeColumn;
    QCheckBox* useOldTokens;
    QCheckBox* useMemory;
    QLineEdit* maxMemoryLine;
    QLineEdit* maxDuplicateLine;
    QCheckBox* useVariableNeigh;
    QLineEdit* variableLine;

    QPushButton* loadGraphButton;
    QPushButton* saveGraphButton;

    QTableWidget* oldTokensTable;
    QPushButton* addRowToken;
    QPushButton* removeRowToken;
    QPushButton* addColumnToken;
    QPushButton* removeColumnToken;

    std::shared_ptr<CoordGraph> g;
    GraphWindow* gw;


};


#endif //CUBE_GRAPHCONTROLWINDOW_H
