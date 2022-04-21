#ifndef CUBE_GAPWINDOW_H
#define CUBE_GAPWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QString>
#include <QCloseEvent>

class GAPWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GAPWindow(QWidget *parent = nullptr);

private slots:
    void loadGraph();
    void selectSaveDir();
    void updateTimeUse(int state);
    void updateRandomUse(int state);
    void updateText(const QString& txt);
    void startComputation();
    void enableStart();

signals:
    void closed();

private:
    QLabel* result_label;
    QPushButton* load_graph_button;
    QLineEdit* load_graph_line;
    QPushButton* select_save_dir_button;
    QLineEdit* select_save_dir_line;
    QLineEdit* weight_key;
    QLineEdit* max_distance;
    QLineEdit* num_categories;
    QCheckBox* time_used;
    QLineEdit* time_key;
    QLineEdit* start_time;
    QLineEdit* max_time;
    QLineEdit* num_time_categories;
    QCheckBox* random_used;
    QLineEdit* max_value_for_random;
    QPushButton* start_button;

    void closeEvent(QCloseEvent *bar);
};


#endif //CUBE_GAPWINDOW_H
