#ifndef CUBE_GRAPHFRAME_H
#define CUBE_GRAPHFRAME_H

#include <QWidget>
#include <memory>
#include <QPoint>
#include <QMouseEvent>

#include "../Graph.h"

class GraphFrame : public QWidget {
    Q_OBJECT
public:
    explicit GraphFrame(const std::shared_ptr<CoordGraph> &g,
                        const double &x = 0.,
                        const double &y = 0.,
                        const double &d = 1.,
                        const double &nr = 5.,
                        QWidget *parent = nullptr);

    void setXtranslation(double x);

    void setYtranslation(double y);

    void setDilatation(double  const& d);

    void setRadius(double const& r);

    void setGraph(const std::shared_ptr<CoordGraph> &new_graph);

    void setEditColor(const std::string &color);

    void setEditable(bool b);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    std::shared_ptr<CoordGraph> graph;
    double xtranslation;
    double ytranslation;
    double dilatation;
    double nodeRadius;
    std::string editColor = "#00ff00";
    std::shared_ptr<QPoint>editPoint;
    bool editable = false;

};


#endif //CUBE_GRAPHFRAME_H
