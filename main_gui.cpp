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
    connect(core, &Core::newConsoleMessage, this, &MainGUI::updateConsole);
}

MainGUI::~MainGUI()
{
    delete ui;
}

void MainGUI::on_startButton_clicked()
{
    /* seed, height, width, entities, genes, gene_string, generations, steps, speed */
    core->initialize(ui->seedSpinBox->value(),
                     ui->worldHeightSpinBox->value(),
                     ui->worldWidthSpinBox->value(),
                     ui->entitiesSpinBox->value(),
                     ui->genesSpinBox->value(),
                     ui->geneEdit->text(),
                     ui->generationsSpinBox->value(),
                     ui->stepsSpinBox->value(),
                     ui->speedSpinBox->value());

    core->updateFitnessCutoff(ui->fitnessSpinBox->value());
    core->updateFitness(ui->consonanceSpinBox->value(), ui->disonanceSpinBox->value(),
                        ui->activitySpinBox->value(), ui->inactivitySpinBox->value());
    core->toggleGenerationExport(ui->exportButton->isChecked());

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

void MainGUI::updateConsole(QString message)
{
    ui->console->append(message);
}

void MainGUI::on_speedSlider_valueChanged(int value)
{
    ui->speedSpinBox->setValue(value);
    core->setCurrentSpeed(value);
}

void MainGUI::on_fitnessSlider_valueChanged(int value)
{
    double actual_value = (double)value / 100;
    ui->fitnessSpinBox->setValue(actual_value);
    core->updateFitnessCutoff(actual_value);
}

void MainGUI::on_consonanceSlider_valueChanged(int value)
{   
    double actual_value = (double)value / 100;
    double others_total = ui->disonanceSpinBox->value() +
                          ui->activitySpinBox->value() +
                          ui->inactivitySpinBox->value();

    if (others_total + actual_value > 1.0) actual_value = 1.0 - others_total; // dej v eno metodo k vrne drug parametr

    ui->consonanceSpinBox->setValue(actual_value);
    ui->consonanceSlider->setValue(actual_value * 100);

    updateFitnessGUI();
}

void MainGUI::on_disonanceSlider_valueChanged(int value)
{
    double actual_value = (double)value / 100;
    double others_total = ui->consonanceSpinBox->value() +
                          ui->activitySpinBox->value() +
                          ui->inactivitySpinBox->value();

    if (others_total + actual_value > 1.0) actual_value = 1.0 - others_total;

    ui->disonanceSpinBox->setValue(actual_value);
    ui->disonanceSlider->setValue(actual_value * 100);

    updateFitnessGUI();
}

void MainGUI::on_activitySlider_valueChanged(int value)
{
    double actual_value = (double)value / 100;
    double others_total = ui->consonanceSpinBox->value() +
                          ui->disonanceSpinBox->value() +
                          ui->inactivitySpinBox->value();

    if (others_total + actual_value > 1.0) actual_value = 1.0 - others_total;

    ui->activitySpinBox->setValue(actual_value);
    ui->activitySlider->setValue(actual_value * 100);

    updateFitnessGUI();
}

void MainGUI::on_inactivitySlider_valueChanged(int value)
{
    double actual_value = (double)value / 100;
    double others_total = ui->consonanceSpinBox->value() +
                          ui->disonanceSpinBox->value() +
                          ui->activitySpinBox->value();

    if (others_total + actual_value > 1.0) actual_value = 1.0 - others_total;

    ui->inactivitySpinBox->setValue(actual_value);
    ui->inactivitySlider->setValue(actual_value * 100);

    updateFitnessGUI();
}

void MainGUI::updateFitnessGUI()
{
    core->updateFitness(ui->consonanceSpinBox->value(), ui->disonanceSpinBox->value(),
                        ui->activitySpinBox->value(), ui->inactivitySpinBox->value());
}

void MainGUI::on_exportButton_toggled(bool checked)
{
    core->toggleGenerationExport(checked);
}
