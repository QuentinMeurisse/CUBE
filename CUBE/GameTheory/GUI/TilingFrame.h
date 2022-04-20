#ifndef CUBE_TILINGFRAME_H
#define CUBE_TILINGFRAME_H

#include <QFrame>
#include <memory>
#include "../Tiling.h"

class TilingFrame: public QFrame {
    Q_OBJECT
public:
    explicit TilingFrame(std::shared_ptr<Tiling> t, const double &d = 1, QWidget *parent = nullptr);
    explicit TilingFrame(QWidget* parent = nullptr);

    void setT(const std::shared_ptr<Tiling> &other);

    void setDilatation(double d);

    void setXtranslation(double x);

    void setYtranslation(double y);

    [[nodiscard]] const std::shared_ptr<Tiling> &getT() const;

public slots:
    void doRepaint();

protected:
    void paintEvent(QPaintEvent *event) override;
    std::shared_ptr<Tiling> t;
    double dilatation = 10;
    double xtranslation = 0;
    double ytranslation = 0;

};


#endif //CUBE_TILINGFRAME_H
