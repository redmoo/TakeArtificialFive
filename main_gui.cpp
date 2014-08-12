#include "main_gui.h"
#include "ui_maingui.h"

MainGUI::MainGUI(Core *app_core, QWidget *parent)
    : core(app_core)
    , ui(new Ui::MainGUI)
    , QMainWindow(parent)
{
    ui->setupUi(this);
    setWindowTitle("Take Artificial Five");

    core->initialize(10, 10, 2, 2, 10, 10, 160);
}

MainGUI::~MainGUI()
{
    delete ui;
}
