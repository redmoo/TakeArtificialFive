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

void LonelyGene::resetGene()
{

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

void SeekingGene::resetGene()
{

}

/** CHORD GENE **/

ChordGene::ChordGene(int steps)
    : total_steps(steps)
{
    interval.initialize(0, 12); // mors dat sanso da ne zaigra nic, torej toni+1 -> random nared da 0 je 0 kr zakaj bi zaigral isti ton?
    duration.initialize(1, 8);
    movement_delta[1].initialize(-1, 1);
    movement_delta[0].initialize(-1, 1);

    signs.clear(); // al je dolzina steps pa se vsakic resetira al pa je fixna dolzina pa pol se vrti okol
    sign_counter = 0;

    for (int i = 0; i < steps; i++) {
        signs.push_back(RandomGenerator::get()->random01() > 0.5 ? 1 : -1);
    }
}

bool ChordGene::trigger(QVector<Entity *> neighbours)
{
    return neighbours.size() == 1; // nared za vec pa da prlagaja ton;
}

QVector3D ChordGene::generateTone(QVector<Entity *> neighbours)
{
    // test, da obraca sign k pride cez mejo, da obraca na X dob, al pa da odigra oktavo visji/nizji
    int tone;
    //int sign = RandomGenerator::get()->random01() > 0.5 ? 1 : -1;
    int sign = signs.at(sign_counter++);

    if (neighbours.at(0)->getCurrentTone() != midi_state::PAUSE) {
        tone = neighbours.at(0)->getCurrentTone() + (sign * interval.getValue());
        if (tone < 0) tone += 12;
        else if (tone > 127) tone -= 12;
    }
    else {
        tone = midi_state::PAUSE;
    }

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

    for (int i = 0; i < total_steps; i++) {
        if (RandomGenerator::get()->random01() <= mutation_rate) {
            signs[i] = RandomGenerator::get()->random01() > 0.5 ? 1 : -1;
        }
    }

}

void ChordGene::resetGene()
{
    sign_counter = 0;
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

void QuietGene::resetGene()
{

}
