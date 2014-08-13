#include "genes.h"

/** LONELY GENE **/

LonelyGene::LonelyGene()
    : default_tone(RandomGenerator::get()->random(48, 72))
    , duration(RandomGenerator::get()->random(1, 8))
{
    movement_delta = QVector2D(RandomGenerator::get()->random(-1, 1), RandomGenerator::get()->random(-1, 1));
}

bool LonelyGene::trigger(QVector<Entity *> neighbours)
{
    return neighbours.size() == 0;
}

QVector3D LonelyGene::generateTone(QVector<Entity *> neighbours)
{
    return QVector3D(default_tone, 100, duration);
}

QVector2D LonelyGene::generateMovementDelta()
{
    return movement_delta;
}

void LonelyGene::mutateParameters(double mutation_rate)
{
    default_tone = RandomGenerator::get()->random(48, 72);
    duration = RandomGenerator::get()->random(1, 4);
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
