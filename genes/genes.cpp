#include "genes.h"

/** LONELY GENE **/

LonelyGene::LonelyGene()
{
    default_tone.initialize(48, 72); // mors dat sanso da ne zaigra nic, torej toni+1 -> random
    duration.initialize(1, 8);
    movement_delta[1].initialize(-1, 1);
    movement_delta[0].initialize(-1, 1);

}

bool LonelyGene::trigger(QVector<Entity *> neighbours)
{
    return neighbours.size() == 0; // dej da je nek counter k se reseta pac ce faila enkrat pol za to rundo odigra nic za x cajta... naslednic gre naprej in cez Y spet 0
}

QVector3D LonelyGene::generateTone(QVector<Entity *> neighbours)
{
    return QVector3D(default_tone.getValue(), 100, duration.getValue());
}

QVector2D LonelyGene::generateMovementDelta()
{
    return QVector2D(movement_delta[0].getValue(), movement_delta[1].getValue());
}

void LonelyGene::mutateParameters(double mutation_rate)
{
    mutateBase(mutation_rate);
    if (RandomGenerator::get()->random01() <= mutation_rate) default_tone.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) duration.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) {
        movement_delta[0].mutate();
        movement_delta[1].mutate();
    }
}

/** SEEKING GENE **/

SeekingGene::SeekingGene()
{}

bool SeekingGene::trigger(QVector<Entity *> neighbours)
{
    return false;
}

QVector3D SeekingGene::generateTone(QVector<Entity *> neighbours)
{
    return QVector3D();
}

QVector2D SeekingGene::generateMovementDelta()
{
    return QVector2D();
}

void SeekingGene::mutateParameters(double mutation_rate)
{

}
