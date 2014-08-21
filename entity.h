#ifndef ENTITY_H
#define ENTITY_H

#include "genes/gene.h" // genes.h?

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

namespace entity_state
{
    const int IDLE = 1000;
    const int PLAYING = 1001;
}

class Gene;

class Entity : public QObject
{
    Q_OBJECT

    friend class Core;

public:
    explicit Entity(int instr, int pat, QVector2D pos, QObject *parent = 0);

    int getInstrument();
    int getPatch();
    QVector<int> getTrack();
    int getNumberOfGenes();

    int getCurrentTone();

private:
    int state;

    int instrument;
    int patch;
    QVector<int> track;

    int current_tone;
    int loudness;
    int beat_counter; // a moram pr resetu positionov tut to resetat?
    //int last_played_tone; // to lahko dobimo iz metode + track record

    QVector2D initial_position;
    QVector2D position;
    QVector2D position_delta;
    double score;
    double consonant_score;
    double disonant_score;
    double activity_score;
    double inactivity_score;
    double tone_score;
    double rhythm_score;

    double mutation_rate; // nared se za reproduction

    // gene list
    QVector<Gene *> genes;

};

#endif // ENTITY_H
