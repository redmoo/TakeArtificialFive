#include "main_gui.h"
#include "ui_maingui.h"

MainGUI::MainGUI(Core *app_core, QWidget *parent)
    : core(app_core)
    , ui(new Ui::MainGUI)
    , QMainWindow(parent)
{
    ui->setupUi(this);
    setWindowTitle("Take Artificial Five");

    core->initialize(20, 20, 4, 2, 10, 321, 160);
}

MainGUI::~MainGUI()
{
    delete ui;
}
