#ifndef ENTITY_H
#define ENTITY_H

#include "genes/genes.h"

#include <QObject>
#include <QVector>
#include <QVector2D>

namespace entity_status
{
    const int IDLE = 1000;
    const int PLAYING = 1001;
}

class Entity : public QObject
{
    Q_OBJECT

public:
    explicit Entity(int instr, int pat, QVector2D pos, QObject *parent = 0);

    int getInstrument() { return instrument; }
    int getPatch() { return patch; }
    QVector2D getPosition() { return position; }

    void addGene(Gene gene); // a je to pametn al enostavn dam da je friend class pa basta

private:
    int status;
    int beat_counter;

    int instrument;
    int patch;
    QVector<int> track;

    int current_tone;
    //int last_played_tone; // to lahko dobimo iz metode + track record

    QVector2D position;
    QVector2D position_queue;
    double score;

    // gene list
    QVector<Gene> genes;

};

#endif // ENTITY_H
