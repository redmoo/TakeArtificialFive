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

private:
    Ui::MainGUI *ui;
    Core *core;

};

#endif // MAINGUI_H
