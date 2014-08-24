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
    void initialize(time_t seed, int w_height, int w_width, int nr_of_entities, int nr_of_genes, QString g_string, int generations, int steps, int speed);
    void pauseSimulation();
    void resumeSimulation();
    void exportWorldPositions(); // vraca 2d tabelo

    void setCurrentSpeed(int speed);
    void updateFitnessCutoff(double cutoff);
    void updateMutationFactor(double mutation);
    void toggleInitialPositionMutation(bool mutate);
    void updateFitness(double consonance, double disonance,
                       double activity, double inactivity,
                       double tonality, double tonality_max, bool tonality_bin,
                       double rhythm, double rhythm_max, bool rhythm_bin);
    void toggleGenerationExport(bool export_current, int loops);
    void toggleFastForward(bool ff);
    void setTransposition(int t);

signals:
    void worldChanged(QVector<int> world, int height, int width);
    void simulationCountChanged(int generation, int step);
    void seedChanged(time_t seed);
    void newConsoleMessage(QString message);

private:    
    void processNextStep();
    void evaluateEntities();
    void mutateEntities();
    void entityCrossover();
    void crossover(Entity *father, Entity *mother);
    void displayScores();
    void resetEntities();
    void sortEntities();
    void sortGenes(Entity *ent);
    void stopCurrentTones();
    void assembleCurrentTrack();
    bool geneStringValidation(QString g_string);

    Gene* initializeRandomGene(int index = -1);
    void assignGenes(int amount, Entity *ent, QString g_string = QString());
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
    int total_genes;
    int number_of_generations;
    int generation_counter;
    int steps_per_generation;
    int step_counter;
    int speed_ms;
    int current_speed;
    bool fast_forward;
    int transposition_queue;
    int transposition;

    double fitness_cutoff;
    double mutation_factor;
    bool mutate_initial_position;

    double consonance_coeff;
    double disonance_coeff;
    double activity_coeff;
    double inactivity_coeff;

    double tone_coeff;
    double tone_max_similarity;
    bool tone_binary;

    double rhythm_coeff;
    double rhythm_max_similarity;
    bool rhythm_binary;

    double neutral_coeff;

    QVector<Entity *> entities;
    QVector<QVector<int>> current_track;

    static const int instruments[7];

    MidiEngine midi_engine;
    int timer_id;

    bool export_current_generation;
    int export_loops;

};

#endif // CORE_H
