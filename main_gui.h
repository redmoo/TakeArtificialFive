#ifndef MAINGUI_H
#define MAINGUI_H

#include "core.h"

#include <QMainWindow>

namespace Ui {
class MainGUI;
}

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGUI(Core *app_core, QWidget *parent = 0);
    ~MainGUI();

public slots:
    void updateLCDs(int generation, int step);
    void updateSeed(time_t seed);
    void updateConsole(QString message);

private slots:
    void on_startButton_clicked();
    void on_pauseButton_toggled(bool checked);

    void on_speedSlider_valueChanged(int value);

    void on_fitnessSlider_valueChanged(int value);

    void on_consonanceSlider_valueChanged(int value);

    void on_activitySlider_valueChanged(int value);

    void on_exportButton_toggled(bool checked);

    void on_disonanceSlider_valueChanged(int value);

    void on_inactivitySlider_valueChanged(int value);

    void on_tonalSlider_valueChanged(int value);

    void on_rhythmicalSlider_valueChanged(int value);

    void on_mutationSlider_valueChanged(int value);

    void on_positionCheckBox_toggled(bool checked);

    void on_fastForwardButton_clicked(bool checked);

    void on_tonalBinaryCheckBox_clicked(bool checked);

    void on_tonalMaxSlider_valueChanged(int value);

    void on_rhythmicalMaxSlider_valueChanged(int value);

    void on_rhythmicalBinaryCheckBox_clicked(bool checked);

    void on_transposeSlider_valueChanged(int value);

private:
    void updateFitnessGUI();

private:
    Ui::MainGUI *ui;
    Core *core;

};

#endif // MAINGUI_H
