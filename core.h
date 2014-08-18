#ifndef CORE_H
#define CORE_H

#include "midi_engine.h"
#include "entity.h"
#include "genes/genes.h"

#include <QObject>
#include <QTimer>

namespace simulation_state
{
    const int IDLE = 7000;
    const int RUNNING = 7001;
    const int PAUSED = 7002;
}

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = 0);

public:
    void initialize(time_t seed, int w_height, int w_width, int nr_of_entities, int nr_of_genes, int generations, int steps, int speed);
    void pauseSimulation();
    void resumeSimulation();
    void exportWorldPositions(); // vraca 2d tabelo

    void setCurrentSpeed(int speed);
    void updateFitnessCutoff(double cutoff);
    void updateFitness(double consonance, double activity);
    void toggleGenerationExport(bool export_current);

signals:
    void worldChanged(QVector<int> world, int height, int width);
    void simulationCountChanged(int generation, int step);
    void seedChanged(time_t seed);
    void newConsoleMessage(QString message);

private:    
    void processNextStep();
    void evaluateEntities();
    void mutateEntities();
    void displayScores();
    void resetEntities();
    void stopCurrentTones();
    void assembleCurrentTrack();

    Gene* initializeRandomGene();
    void assignGenes(int amount, Entity *ent);
    QVector2D generateInitialPosition();

    QVector<Entity *> getEntityNeighbours(Entity *parent, int radius);
    bool inRadius(QVector2D center, QVector2D neighbour, int radius);
    int entityPresent(int row, int column);

    void timerEvent(QTimerEvent *);

private:
    int state;

    int world_height;
    int world_width;

    int number_of_entities;
    int number_of_genes;
    int number_of_generations;
    int generation_counter;
    int steps_per_generation;
    int step_counter;
    int speed_ms;
    int current_speed;

    double fitness_cutoff;
    double consonance_coeff;
    double activity_coeff;
    double neutral_coeff;

    QVector<Entity *> entities;
    QVector<QVector<int>> current_track;

    static const int instruments[7];

    MidiEngine midi_engine;
    int timer_id;

    bool export_current_generation;

};

#endif // CORE_H
