#include "genes.h"

#include "../midi_engine.h"

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

/** CHORD GENE **/

ChordGene::ChordGene()
{
    interval.initialize(0, 12); // mors dat sanso da ne zaigra nic, torej toni+1 -> random
    duration.initialize(1, 8);
    movement_delta[1].initialize(-1, 1);
    movement_delta[0].initialize(-1, 1);
}

bool ChordGene::trigger(QVector<Entity *> neighbours)
{
    return neighbours.size() == 1; // nared za vec pa da prlagaja ton;
}

QVector3D ChordGene::generateTone(QVector<Entity *> neighbours)
{
    int sign = RandomGenerator::get()->random01() > 0.5 ? 1 : -1;
    int tone = neighbours.at(0)->getCurrentTone() != midi_state::PAUSE ? neighbours.at(0)->getCurrentTone() + (sign * interval.getValue()) : midi_state::PAUSE; // a sm za 1 dobo?
    return QVector3D(tone, 100, duration.getValue());
}

QVector2D ChordGene::generateMovementDelta()
{
    return QVector2D(movement_delta[0].getValue(), movement_delta[1].getValue());
}

void ChordGene::mutateParameters(double mutation_rate)
{
    mutateBase(mutation_rate);
    if (RandomGenerator::get()->random01() <= mutation_rate) interval.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) duration.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) {
        movement_delta[0].mutate();
        movement_delta[1].mutate();
    }
}


QuietGene::QuietGene()
{
    duration.initialize(1, 8);
    movement_delta[1].initialize(-1, 1);
    movement_delta[0].initialize(-1, 1);
}

bool QuietGene::trigger(QVector<Entity *> neighbours)
{
    return neighbours.size() == 0;
}

QVector3D QuietGene::generateTone(QVector<Entity *> neighbours)
{
    return QVector3D(midi_state::PAUSE, 0, duration.getValue());
}

QVector2D QuietGene::generateMovementDelta()
{
    return QVector2D(movement_delta[0].getValue(), movement_delta[1].getValue());
}

void QuietGene::mutateParameters(double mutation_rate)
{
    mutateBase(mutation_rate);
    if (RandomGenerator::get()->random01() <= mutation_rate) duration.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) {
        movement_delta[0].mutate();
        movement_delta[1].mutate();
    }
}
