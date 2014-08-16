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
    GeneParameter default_tone;
    GeneParameter duration;
    GeneParameter movement_delta[2];

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
