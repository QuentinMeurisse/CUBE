#include "TilingFrame.h"

#include <utility>
#include <QPainter>
#include <QPolygonF>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <string>
#include <iostream>

using namespace std;
using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;

TilingFrame::TilingFrame(std::shared_ptr<Tiling> t, const double &d, QWidget *parent) : QFrame(parent){
    this->t = std::move(t);
    dilatation = d;
    int w = ceil(this->t->getWidth() * d);
    int h = ceil(this->t->getHeight() * d);

    resize(w, h);
    setFrameShape(QFrame::Box);
}

TilingFrame::TilingFrame(QWidget *parent): QFrame(parent) {
    resize(750, 750);
    setFrameShape(QFrame::Box);

}

void TilingFrame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (t) {
        setStyleSheet("border: 0px solid black;");
        QPainter painter(this);

        painter.setPen(QPen(QBrush("#000000"), 0.5));
        painter.setRenderHint(QPainter::Antialiasing);

        for (unsigned int i = 0; i < t->numCells(); ++i) {
            QPolygonF polygon;
            for (auto p : t->vertexOf(i)) {
                polygon << QPointF(dilatation * p.first + xtranslation, dilatation * p.second + ytranslation);
            }
            string color = t->getColor(i);
            QString qcolor = QString::fromStdString(color);
            painter.setBrush(QBrush(QColor(qcolor)));
            painter.drawConvexPolygon(polygon);
        }
    } else{
        QPainter painter(this);

        painter.setPen(QPen(QBrush("#000000"), 0.5));
        painter.setRenderHint(QPainter::Antialiasing);

        painter.drawText(QRect(0, 0, 750, 750), Qt::AlignCenter,"Tiling must be configured");
        setStyleSheet("border: 1px solid black;");

    }
}

void TilingFrame::doRepaint(){
    repaint();
}

void TilingFrame::setT(const shared_ptr<Tiling> &other) {
    this->t = other;
}

const shared_ptr<Tiling> &TilingFrame::getT() const {
    return t;
}

void TilingFrame::setDilatation(double d) {
    this->dilatation = d;
}

void TilingFrame::setXtranslation(double x) {
    TilingFrame::xtranslation = x;
}

void TilingFrame::setYtranslation(double y) {
    TilingFrame::ytranslation = y;
}
