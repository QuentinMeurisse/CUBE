#ifndef CUBE_DISPLAYTILINGFRAME_H
#define CUBE_DISPLAYTILINGFRAME_H

#include "TilingFrame.h"

#include <QMainWindow>
#include <memory>

//todo: delete
class DisplayTilingFrame : public QMainWindow {
    Q_OBJECT
public:
    DisplayTilingFrame(std::shared_ptr<Tiling> t, int d, QWidget *parent = nullptr);

private:
    TilingFrame* frame;

};


#endif //CUBE_DISPLAYTILINGFRAME_H
