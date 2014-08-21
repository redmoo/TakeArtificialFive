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
    void resetGene();

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
    void resetGene();

private:
};

class ChordGene : public Gene
{

public:
    explicit ChordGene(int steps);

    bool trigger(QVector<Entity*> neighbours);
    QVector3D generateTone(QVector<Entity*> neighbours);
    QVector2D generateMovementDelta();

    void mutateParameters(double mutation_rate);
    void resetGene();

private:
    int total_steps;
    QVector<int> signs;
    int sign_counter;

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
    void resetGene();

private:
    GeneParameter duration;
    GeneParameter movement_delta[2];

};


#endif // GENES_H
