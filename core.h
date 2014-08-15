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
    void initialize(int w_height, int w_width, int nr_of_entities, int nr_of_genes, int generations, int steps, int speed);
    void pauseSimulation();
    void resumeSimulation();
    void exportWorldPositions(); // vraca 2d tabelo

signals:
    void worldChanged(QVector<int> world, int height, int width);

private:    
    void processNextStep();

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
    int steps_per_generation;
    int step_counter;
    int speed_ms;

    QVector<Entity *> entities;

    static const int instruments[5];

    MidiEngine midi_engine;
    int timer_id;

};

#endif // CORE_H
