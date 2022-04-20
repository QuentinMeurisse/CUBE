#include "gap_window.h"
#include "gap_config.h"
#include "gapworker.h"

#include <QGroupBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QThread>
#include <QMessageBox>
#include <iostream>
#include <utility>

using namespace std;

GapWindow::GapWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("GAP");
    resize(1000, 800);

    auto* central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    auto* text_frame = new QFrame(central_widget);
    result_label = new QLabel("Must be configured", text_frame);
    text_frame->move(20, 550);
    text_frame->resize(500, 200);
    text_frame->setFrameStyle(QFrame::Box);
    result_label->move(10, 10);
    result_label->resize(480, 180);

    auto* file_group = new QGroupBox("Files", central_widget);
    load_graph_button = new QPushButton("Load graph", file_group);
    load_graph_button->setToolTip("Load a graph from JSON file");
    load_graph_line = new QLineEdit("/home/quentin/Documents/GIS/mons-time without "
                                    "holidays/mons_7h_10h.json", file_group);
    load_graph_line->setFixedWidth(200);

    select_save_dir_button = new QPushButton("Save dir", file_group);
    select_save_dir_line = new QLineEdit("/home/quentin/Bureau/gap_test", file_group);
    select_save_dir_line->setFixedWidth(200);

    auto* file_layout = new QGridLayout;
    file_layout->addWidget(load_graph_button, 0, 0);
    file_layout->addWidget(load_graph_line, 0, 1);
    file_layout->addWidget(select_save_dir_button, 1, 0);
    file_layout->addWidget(select_save_dir_line, 1, 1);

    file_group->setLayout(file_layout);
    file_group->move(20, 20);

    auto* config_group = new QGroupBox("Configuration", central_widget);

    auto* dist_group = new QGroupBox("Distance", config_group);
    weight_key = new QLineEdit("length", dist_group);
    weight_key->setFixedWidth(100);
    max_distance = new QLineEdit("2000", dist_group);
    max_distance->setFixedWidth(100);
    num_categories = new QLineEdit("5", dist_group);
    num_categories->setFixedWidth(100);

    auto* dist_group_layout = new QFormLayout;
    dist_group_layout->addRow("Weight key", weight_key);
    dist_group_layout->addRow("Max distance", max_distance);
    dist_group_layout->addRow("Num categories", num_categories);
    dist_group->setLayout(dist_group_layout);

    auto* time_group = new QGroupBox("Times", config_group);
    time_used = new QCheckBox("Use time", time_group);
    auto* time_form = new QWidget(time_group);
    time_key = new QLineEdit("times", time_form);
    time_key->setFixedWidth(100);
    time_key->setEnabled(false);
    start_time = new QLineEdit("3600", time_form);
    start_time->setFixedWidth(100);
    start_time->setEnabled(false);
    max_time = new QLineEdit("1800", time_form);
    max_time->setFixedWidth(100);
    max_time->setEnabled(false);
    num_time_categories = new QLineEdit("5", time_form);
    num_time_categories->setFixedWidth(100);
    num_time_categories->setEnabled(false);

    auto* time_form_layout = new QFormLayout;
    time_form_layout->addRow("Times key", time_key);
    time_form_layout->addRow("Start time", start_time);
    time_form_layout->addRow("Max time", max_time);
    time_form_layout->addRow("Num categories", num_time_categories);
    time_form->setLayout(time_form_layout);

    auto* time_layout = new QVBoxLayout;
    time_layout->addWidget(time_used);
    time_layout->addWidget(time_form);
    time_group->setLayout(time_layout);

    auto* random_group = new QGroupBox("Random preference", config_group);
    random_used = new QCheckBox("Use random preference");
    auto* random_form = new QWidget(random_group);
    max_value_for_random = new QLineEdit("5", random_form);
    max_value_for_random->setFixedWidth(100);
    max_value_for_random->setEnabled(false);

    auto* random_form_layout = new QFormLayout;
    random_form_layout->addRow("Max value", max_value_for_random);
    random_form->setLayout(random_form_layout);

    auto* random_layout = new QVBoxLayout;
    random_layout->addWidget(random_used);
    random_layout->addWidget(random_form);
    random_group->setLayout(random_layout);


    auto* config_layout = new QHBoxLayout;
    config_layout->addWidget(dist_group);
    config_layout->addWidget(time_group);
    config_layout->addWidget(random_group);
    config_group->setLayout(config_layout);
    config_group->move(20, 200);

    start_button = new QPushButton("START", central_widget);
    start_button->move(600, 600);
    start_button->setFixedSize(200, 50);

    connect(load_graph_button, &QPushButton::clicked, this, &GapWindow::loadGraph);
    connect(select_save_dir_button, &QPushButton::clicked, this, &GapWindow::selectSaveDir);
    connect(time_used, &QCheckBox::stateChanged, this, &GapWindow::updateTimeUse);
    connect(random_used, &QCheckBox::stateChanged, this, &GapWindow::updateRandomUse);
    connect(start_button, &QPushButton::clicked, this, &GapWindow::startComputation);



}

void GapWindow::loadGraph() {
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Choose graph file"),
                                                     "",
                                                     tr("JSON files (*.json)"),
                                                     nullptr, QFileDialog::DontUseNativeDialog);

    load_graph_line->setText(file_name);
}

void GapWindow::selectSaveDir() {
    QString save_dir = QFileDialog::getExistingDirectory(this,
                                                         tr("Choose directory"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);
    select_save_dir_line->setText(save_dir);
}

void GapWindow::updateTimeUse(int state) {
    time_key->setEnabled(state == Qt::Checked);
    start_time->setEnabled(state == Qt::Checked);
    max_time->setEnabled(state == Qt::Checked);
    num_time_categories->setEnabled(state == Qt::Checked);
}

void GapWindow::updateRandomUse(int state) {
    max_value_for_random->setEnabled(state == Qt::Checked);
}

void GapWindow::updateText(const QString& txt) {
    result_label->setText(txt);
}

void GapWindow::startComputation() {
    QString error_details;
    Config config;
    config.num_criteria++;
    QString load_graph_str = load_graph_line->text();
    if (load_graph_str.isEmpty())
        error_details.append("- The graph is not loaded.\n");
    config.graph_path = load_graph_str.toStdString();

    QString save_dir_str = select_save_dir_line->text();
    if (save_dir_str.isEmpty())
        error_details.append("- The save dir is missing.\n");
    config.save_dir = save_dir_str.toStdString();

    QString weight_key_str = weight_key->text();
    if (weight_key_str.isEmpty())
        error_details.append("- The weight key is missing.\n");
    config.weight_key = weight_key_str.toStdString();

    QString max_distance_str = max_distance->text();
    if (max_distance_str.isEmpty())
        error_details.append("- The max distance used to categorize the distances is missing.\n");
    config.max_dist = max_distance_str.toInt();

    QString num_categories_str = num_categories->text();
    if (num_categories_str.isEmpty())
        error_details.append("- The number of categories of distance is missing.\n");
    config.num_dist_categories = num_categories_str.toInt();

    if (time_used->isChecked()){
        QString times_key_str = time_key->text();
        config.num_criteria++;
        if (times_key_str.isEmpty())
            error_details.append("- The time key is missing.\n");
        config.times_key = times_key_str.toStdString();

        QString start_time_str = start_time->text();
        if (start_time_str.isEmpty())
            error_details.append("- The start time is missing.\n");
        config.start_time = start_time_str.toInt();

        QString max_time_str = max_time->text();
        if (max_time_str.isEmpty())
            error_details.append("- The max time used to categorize the time travel is missing.\n");
        config.max_time = max_time_str.toInt();

        QString num_t_cat_str = num_time_categories->text();
        if (num_categories_str.isEmpty())
            error_details.append("- The number of categories of time travel is missing.\n");
        config.num_time_categories = num_t_cat_str.toInt();
    }

    if (random_used->isChecked()){
        QString max_rnd_str = max_value_for_random->text();
        config.num_criteria++;
        if (max_rnd_str.isEmpty())
            error_details.append("- The maximum profit value for random preference is missing.\n");
        config.max_value_for_random = max_rnd_str.toInt();
    }

    if (error_details.isEmpty()){
        start_button->setEnabled(false);

        auto* q_thread = new QThread;
        auto* worker = new GAPWorker(config);
        worker->moveToThread(q_thread);
        connect(q_thread,&QThread::started, worker, &GAPWorker::process);
        connect(worker, &GAPWorker::infoChanged, this, &GapWindow::updateText);
        connect(worker, &GAPWorker::finished, q_thread, &QThread::quit);
        connect(worker, &GAPWorker::finished, this, &GapWindow::enableStart);
        connect(worker, &GAPWorker::finished, worker, &GAPWorker::deleteLater);
        connect(q_thread, &QThread::finished, q_thread, &QThread::deleteLater);
        q_thread->start();
    }
    else{
        QMessageBox messageBox;
        messageBox.setText("Something is not configured !");
        messageBox.setDetailedText(error_details);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }



}

void GapWindow::enableStart() {
    start_button->setEnabled(true);
}


