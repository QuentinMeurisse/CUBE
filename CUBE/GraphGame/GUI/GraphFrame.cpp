#include "GraphFrame.h"


#include <string>
#include <cmath>
#include <QPainter>
#include <QPointF>
#include <QString>
#include <iostream>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

using namespace std;

GraphFrame::GraphFrame(const shared_ptr<CoordGraph> &g, const double &x, const double &y, const double &d, const double &nr,
                       QWidget *parent): QWidget(parent) {
    graph = g;
    xtranslation = x;
    ytranslation = y;
    dilatation = d;
    nodeRadius = nr;
    resize(1500, 1000);
}

void GraphFrame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 1));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0, 0, 0));


    for (unsigned int i = 0; i < graph->size(); i++){
        uint64_t current = (*graph)[i];
        auto [x, y, color] = graph->getNodeData(current);
        QPointF q1(dilatation * x + xtranslation, dilatation * y + ytranslation);
        for (uint64_t const& adj : graph->getSuccessors(current)){
            if(graph->findNodeIndex(adj) > i) {
                auto [x2, y2] = graph->getCoordinate(adj);
                QPointF q2(dilatation * x2 + xtranslation, dilatation * y2 + ytranslation);
                painter.drawLine(q1, q2);
            }
        }
        if (editPoint and pow(editPoint->x() - q1.x(), 2) + pow(editPoint->y() - q1.y(), 2) <= pow(nodeRadius, 2)) {
            if (color == "#ffffff") {
                graph->setColor(current, editColor);
                QString qcolor = QString::fromStdString(editColor);
                painter.setBrush(QBrush(QColor(qcolor)));
            } else {
                graph->setColor(current, "#ffffff");
                QString qcolor = QString::fromStdString("#ffffff");
                painter.setBrush(QBrush(QColor(qcolor)));
            }
        } else {
            QString qcolor = QString::fromStdString(color);
            painter.setBrush(QBrush(QColor(qcolor)));
        }
        painter.drawEllipse(q1, nodeRadius, nodeRadius);
    }
}

void GraphFrame::setDilatation(const double &d) {
    this->dilatation = d;
}

void GraphFrame::setRadius(const double &r) {
    this->nodeRadius = r;
}

void GraphFrame::setGraph(const shared_ptr<CoordGraph> &new_graph) {
    GraphFrame::graph = new_graph;
}

void GraphFrame::setXtranslation(double x) {
    GraphFrame::xtranslation = x;
}

void GraphFrame::setYtranslation(double y) {
    GraphFrame::ytranslation = y;
}

void GraphFrame::setEditColor(const string &color) {
    GraphFrame::editColor = color;
}

void GraphFrame::mousePressEvent(QMouseEvent *event) {
    if (editable and event->button() == Qt::LeftButton){
        QPoint p = event->pos();
        editPoint = make_shared<QPoint>(p);
        repaint();
        editPoint = nullptr;
    }
}

void GraphFrame::setEditable(bool b) {
    GraphFrame::editable = b;
}
