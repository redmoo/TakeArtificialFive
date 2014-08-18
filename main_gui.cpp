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
    /* seed, height, width, entities, genes, generations, steps, speed */
    core->initialize(ui->seedSpinBox->value(),
                     ui->worldHeightSpinBox->value(),
                     ui->worldWidthSpinBox->value(),
                     ui->entitiesSpinBox->value(),
                     ui->genesSpinBox->value(),
                     ui->generationsSpinBox->value(),
                     ui->stepsSpinBox->value()/*110 321*/,
                     ui->speedSpinBox->value());

    core->updateFitnessCutoff(ui->fitnessSpinBox->value());
    core->updateFitness(ui->consonanceSpinBox->value(), ui->activitySpinBox->value());
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
    double activity = ui->activitySpinBox->value();

    if (actual_value + activity > 1.0) activity = 1.0 - actual_value; // dej v eno metodo k vrne drug parametr
    updateFitnessGUI(actual_value, activity);
}

void MainGUI::on_activitySlider_valueChanged(int value)
{
    double actual_value = (double)value / 100;
    double consonance = ui->consonanceSpinBox->value();

    if (actual_value + consonance > 1.0) consonance = 1.0 - actual_value;
    updateFitnessGUI(consonance, actual_value);
}

void MainGUI::updateFitnessGUI(double consonance, double activity)
{
    ui->consonanceSlider->blockSignals(true); // nared tko da je if consonance+silence < 1.0 -> neutral = 1-(c+s)
    ui->activitySlider->blockSignals(true);

    ui->consonanceSpinBox->setValue(consonance);
    ui->activitySpinBox->setValue(activity);
    ui->consonanceSlider->setValue(consonance * 100);
    ui->activitySlider->setValue(activity * 100);

    ui->consonanceSlider->blockSignals(false);
    ui->activitySlider->blockSignals(false);

    core->updateFitness(consonance, activity);
}

void MainGUI::on_exportButton_toggled(bool checked)
{
    core->toggleGenerationExport(checked);
}
