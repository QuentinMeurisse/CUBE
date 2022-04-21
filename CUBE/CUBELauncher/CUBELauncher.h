#ifndef CUBE_CUBELAUNCHER_H
#define CUBE_CUBELAUNCHER_H

#include "GUI/GameFrame.h"
#include "GUI/GraphControlWindow.h"
#include "FLPWindow.h"
#include "GAPWindow.h"

#include <QMainWindow>
#include <QPushButton>

class CUBELauncher : public QMainWindow{
    Q_OBJECT
public:
    explicit CUBELauncher (QWidget* parent = nullptr);

protected slots:
    void selectGame();
    void selectGraph();
    void selectFLP();
    void selectGAP();
    void reshowLauncher();

private:
    GameFrame* gf;
    GraphControlWindow* gcw;
    FLPWindow* flpw;
    GAPWindow* gapw;

    QPushButton* gameButton;
    QPushButton* graphButton;
    QPushButton* flpButton;
    QPushButton* gapButton;

};


#endif //CUBE_CUBELAUNCHER_H
