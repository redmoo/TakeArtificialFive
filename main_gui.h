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

private slots:
    void on_startButton_clicked();
    void on_pauseButton_toggled(bool checked);

private:
    Ui::MainGUI *ui;
    Core *core;

};

#endif // MAINGUI_H
