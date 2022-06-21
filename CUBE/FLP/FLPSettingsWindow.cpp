#include "FLPSettingsWindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <iostream>
#include <sstream>

using namespace std;

FLPSettingsWindow::FLPSettingsWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(500, 320);
    setWindowTitle(tr("Genetic Search Settings"));

    auto* centralWidget = new QWidget(this);
    centralWidget->setFixedSize(this->width(), this->height());

    int line_width = 150;
    int line_height = 25;

    auto* numGenerationLabel = new QLabel(tr("Number of generation"), centralWidget);
    numGenerationLine = new QLineEdit("500", centralWidget);
    numGenerationLine->setFixedSize(line_width, line_height);

    auto* populationSizeLabel = new QLabel(tr("Population's size"));
    populationSizeLine = new QLineEdit("50", centralWidget);
    populationSizeLine->setFixedSize(line_width, line_height);

    auto* mutationRateLabel = new QLabel(tr("Mutation's rate"));
    mutationRateLine = new QLineEdit("0.01", centralWidget);
    mutationRateLine->setFixedSize(line_width, line_height);


    facilitiesCostCheck = new QCheckBox("Use default school cost", centralWidget);
    facilitiesCostLine = new QLineEdit("5", centralWidget);
    facilitiesCostLine->setFixedSize(line_width, line_height);
    facilitiesCostLine->setEnabled(false);
    connect(facilitiesCostCheck, SIGNAL(stateChanged(int)), this, SLOT(activateDefaultFacilitiesCost(int)));

    transportCostCheck = new QCheckBox("Use default moving cost", centralWidget);

    useConstraintCheck = new QCheckBox("Use distance constraint", centralWidget);
    auto* maxDistLabel = new QLabel("Maximum distance", centralWidget);
    maxDistLine = new QLineEdit("4", centralWidget);
    maxDistLine->setFixedSize(line_width, line_height);
    maxDistLine->setEnabled(false);
    auto* penaltyLabel = new QLabel("Penalty", centralWidget);
    penaltyLine = new QLineEdit("30", centralWidget);
    penaltyLine->setFixedSize(line_width, line_height);
    penaltyLine->setEnabled(false);
    connect(useConstraintCheck, SIGNAL(stateChanged(int)), this, SLOT(activateConstraint(int)));

    okButton = new QPushButton(tr("&OK"), centralWidget);
    cancelButton = new QPushButton(tr("&Cancel"), centralWidget);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

    auto* layout = new QGridLayout;
    layout->addWidget(numGenerationLabel, 0, 0);
    layout->addWidget(numGenerationLine, 0, 1);
    layout->addWidget(populationSizeLabel, 1, 0);
    layout->addWidget(populationSizeLine, 1, 1);
    layout->addWidget(mutationRateLabel, 2, 0);
    layout->addWidget(mutationRateLine, 2, 1);
    layout->addWidget(facilitiesCostCheck, 4, 0);
    layout->addWidget(facilitiesCostLine, 4, 1);
    layout->addWidget(transportCostCheck, 5, 0);
    layout->addWidget(useConstraintCheck, 6, 0);
    layout->addWidget(maxDistLabel, 7, 0);
    layout->addWidget(maxDistLine, 7, 1);
    layout->addWidget(penaltyLabel, 8, 0);
    layout->addWidget(penaltyLine, 8, 1);
    layout->addWidget(okButton, 9, 0);
    layout->addWidget(cancelButton, 9, 1);
    centralWidget->setLayout(layout);
}

void FLPSettingsWindow::cancel() {
    this->close();
}

void FLPSettingsWindow::ok() {
    stringstream ss;
    if (numGenerationLine->text().contains("."))
        ss << "- Number of generation must be an integer" << endl;
    if (populationSizeLine->text().contains("."))
        ss << "- Number of generation must be an integer" << endl;
    double x =  mutationRateLine->text().toDouble();
    if (x < 0 or x > 1)
        ss << "- Mutation's rate must be in [0, 1]" << endl;
    if (ss.str().empty()) {
        settings = {numGenerationLine->text().toInt(),
                    populationSizeLine->text().toInt(),
                     mutationRateLine->text().toDouble(),
                     facilitiesCostLine->text().toDouble(),
                     maxDistLine->text().toDouble(),
                     penaltyLine->text().toDouble()};
        this->close();
    } else{
        QMessageBox messageBox;
        messageBox.setText(QString::fromStdString(ss.str()));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.exec();
    }
}

const FLPSettings &FLPSettingsWindow::getSettings() const {
    return settings;
}

void FLPSettingsWindow::setSettings(const FLPSettings &new_settings) {
    FLPSettingsWindow::settings = new_settings;
}

bool FLPSettingsWindow::useDefaultFacilitiesCost() {
    return facilitiesCostCheck->isChecked();
}

bool FLPSettingsWindow::useDefaultTransportCost() {
    return transportCostCheck->isChecked();
}

void FLPSettingsWindow::activateDefaultFacilitiesCost(int state) {
    facilitiesCostLine->setEnabled(state == Qt::Checked);

}

void FLPSettingsWindow::activateConstraint(int state) {
    maxDistLine->setEnabled(state == Qt::Checked);
    penaltyLine->setEnabled(state == Qt::Checked);
}

bool FLPSettingsWindow::useConstraint() {
    return useConstraintCheck->isChecked();
}

ostream &operator<<(ostream &os, const FLPSettings &settings) {
    os << "numGeneration: " << settings.numGeneration << " populationSize: " << settings.populationSize
       << " mutationRate: " << settings.mutationRate;
    return os;
}
