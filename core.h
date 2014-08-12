#ifndef CORE_H
#define CORE_H

#include "midi_engine.h"
#include "entity.h"
#include "genes/genes.h"

#include <QObject>
#include <QTimer>

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = 0);

public:
    void initialize(int w_height, int w_width, int nr_of_entities, int nr_of_genes, int generations, int steps, int speed);

    void exportWorldPositions(); // vraca 2d tabelo

private:    
    void processNextStep();

    void initializeGenePool();
    void assignGenes(int amount, Entity *ent);
    QVector2D generateInitialPosition();
    bool entityPresent(int row, int column);

    void timerEvent(QTimerEvent *);

private:
    int world_height;
    int world_width;

    int number_of_entities;
    int number_of_genes;
    int number_of_generations;
    int steps_per_generation;
    int speed_ms;

    QVector<Gene> gene_pool;
    QVector<Entity*> entities;

    static const int instruments[5];

    MidiEngine midi_engine;
    int timer_id;

};

#endif // CORE_H
