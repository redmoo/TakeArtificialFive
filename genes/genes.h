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

private:
};

class ChordGene : public Gene
{

public:
    explicit ChordGene();

    bool trigger(QVector<Entity*> neighbours);
    QVector3D generateTone(QVector<Entity*> neighbours);
    QVector2D generateMovementDelta();

    void mutateParameters(double mutation_rate);

private:
    QVector<int> signs;

    GeneParameter interval; // PREVERI KAJ MAJO ENTITETE ZA DEFAULT TON! razn ce ta prever a ni un se nc zaigral
    GeneParameter duration;
    GeneParameter movement_delta[2];

};

class QuietGene : public Gene
{

public:
    explicit QuietGene();

    bool trigger(QVector<Entity*> neighbours);
    QVector3D generateTone(QVector<Entity*> neighbours);
    QVector2D generateMovementDelta();

    void mutateParameters(double mutation_rate);

private:
    GeneParameter duration;
    GeneParameter movement_delta[2];

};


#endif // GENES_H
