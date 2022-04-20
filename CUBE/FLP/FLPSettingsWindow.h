#ifndef CUBE_FLPSETTINGSWINDOW_H
#define CUBE_FLPSETTINGSWINDOW_H


#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <ostream>

struct FLPSettings{
    int numGeneration;
    int populationSize;
    double mutationRate;
    double facilitiesCost;
    double maxDist;
    double penalty;

    friend std::ostream &operator<<(std::ostream &os, const FLPSettings &settings);
};

class FLPSettingsWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit FLPSettingsWindow(QWidget* parent = nullptr);

    [[nodiscard]] const FLPSettings &getSettings() const;

    void setSettings(const FLPSettings &new_settings);

    bool useDefaultFacilitiesCost();

    bool useDefaultTransportCost();

    bool useConstraint();

protected slots:
    void cancel();
    void ok();
    void activateDefaultFacilitiesCost(int state);
    void activateConstraint(int state);

private:
    QLineEdit* numGenerationLine;
    QLineEdit* populationSizeLine;
    QLineEdit* mutationRateLine;
    QCheckBox* facilitiesCostCheck;
    QLineEdit* facilitiesCostLine;
    QCheckBox* transportCostCheck;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QCheckBox* useConstraintCheck;
    QLineEdit* maxDistLine;
    QLineEdit* penaltyLine;

    FLPSettings settings = {500, 50, 0.01};

};


#endif //CUBE_FLPSETTINGSWINDOW_H
