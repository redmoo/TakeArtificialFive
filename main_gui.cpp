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
    connect(core, &Core::simulationCountChanged, this, &MainGUI::updateLCDs);
    connect(core, &Core::seedChanged, this, &MainGUI::updateSeed);

}

MainGUI::~MainGUI()
{
    delete ui;
}

void MainGUI::on_startButton_clicked()
{
    /* seed, height, width, entities, genes, generations, steps, speed */
    core->initialize(ui->seedSpinBox->value(),
                     ui->worldHeightSpinBox->value(),
                     ui->worldWidthSpinBox->value(),
                     ui->entitiesSpinBox->value(),
                     ui->genesSpinBox->value(),
                     ui->generationsSpinBox->value(),
                     ui->stepsSpinBox->value()/*110 321*/,
                     ui->speedSpinBox->value());

    ui->worldHeightSpinBox->setEnabled(false);
    ui->worldWidthSpinBox->setEnabled(false);
    ui->entitiesSpinBox->setEnabled(false);
    ui->genesSpinBox->setEnabled(false);
    ui->generationsSpinBox->setEnabled(false);
    ui->stepsSpinBox->setEnabled(false);
    ui->speedSpinBox->setEnabled(false);
    ui->seedSpinBox->setDisabled(true);

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);

    ui->speedSlider->setValue(ui->speedSpinBox->value());
    ui->speedSlider->setEnabled(true);


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

void MainGUI::updateLCDs(int generation, int step)
{
    ui->generationLCD->display(generation);
    ui->stepLCD->display(step);
}

void MainGUI::updateSeed(time_t seed)
{
    ui->seedSpinBox->setValue(seed);
}

void MainGUI::on_speedSlider_valueChanged(int value)
{
    ui->speedSpinBox->setValue(value);
    core->setCurrentSpeed(value);
}
