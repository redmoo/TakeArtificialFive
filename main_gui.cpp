#include "main_gui.h"
#include "ui_maingui.h"

MainGUI::MainGUI(Core *app_core, QWidget *parent)
    : core(app_core)
    , ui(new Ui::MainGUI)
    , QMainWindow(parent)
{
    ui->setupUi(this);
    setWindowTitle("Take Artificial Five");

    ui->pauseButton->setDisabled(true);

    connect(core, &Core::worldChanged, ui->frame, &WorldFrame::drawWorld);
}

MainGUI::~MainGUI()
{
    delete ui;
}

void MainGUI::on_startButton_clicked()
{
    core->initialize(20, 20, 4, 2, 10, 110/*321*/, 160);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);

}

void MainGUI::on_pauseButton_toggled(bool checked)
{
    if (checked) {
        ui->pauseButton->setText(QString("Resume"));
        core->pauseSimulation();
    } else {
        ui->pauseButton->setText(QString("Pause"));
        core->resumeSimulation();
    }
}
