#ifndef GENES_H
#define GENES_H

#include "gene.h"

class LonelyGene : public Gene
{

public:
    explicit LonelyGene();

    bool trigger(QVector<Entity*> neighbours);
    QVector3D generateTone(QVector<Entity*> neighbours);
    QVector2D generateMovementDelta();

    void mutateParameters(double mutation_rate);

private:
    int default_tone;
    int duration;
    QVector2D movement_delta;

};

class SeekingGene : public Gene
{

public:
    explicit SeekingGene();

    bool trigger(QVector<Entity*> neighbours);
    QVector3D generateTone(QVector<Entity*> neighbours);
    QVector2D generateMovementDelta();

    void mutateParameters(double mutation_rate);
};

#endif // GENES_H
