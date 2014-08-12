#include "core.h"

#include "random_generator.h"

#include <QDebug>
#include <iostream>
#include <ctime>

const int Core::instruments[5] = { 0, 52, 48, 35, 81 }; // 0=piano, 52=choir aahs, 48=strings, 35=fretless bass, 74=pan flute

Core::Core(QObject *parent)
    : QObject(parent)
{}

void Core::initialize(int w_height, int w_width, int nr_of_entities, int nr_of_genes, int generations, int steps, int speed)
{
    world_height = w_height;
    world_width = w_width;

    number_of_entities = nr_of_entities;
    number_of_genes = nr_of_genes;
    number_of_generations = generations;
    steps_per_generation = steps;
    speed_ms = speed;

    // SETUP EVERYTHING ...
    initializeGenePool();

    for (int i = 0; i < number_of_entities; i++) {

        Entity *ent = new Entity(instruments[i % 5], i, generateInitialPosition());
        assignGenes(number_of_genes, ent);
        entities.append(ent);

        midi_engine.setPatch(ent->getPatch(), ent->getInstrument());

    }

    timer_id = startTimer(speed);

}

void Core::processNextStep()
{

}

void Core::exportWorldPositions()
{
    for (int i = 0; i < world_height; i++) {
        for (int j = 0; j < world_width; j++) {
            if (entityPresent(i, j))
                std::cout << " O ";
            else
                std::cout << " - ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Core::initializeGenePool()
{
    gene_pool.append(LonelyGene());
    gene_pool.append(SeekingGene());
}

void Core::assignGenes(int amount, Entity *ent)
{
    for (int i = 0; i < amount; i++) {
        ent->addGene(gene_pool[RandomGenerator::get()->random(0, gene_pool.size()-1)]);
    }
}

QVector2D Core::generateInitialPosition()
{
    return QVector2D(RandomGenerator::get()->random(0, world_height-1),
                     RandomGenerator::get()->random(0, world_width-1));
}

bool Core::entityPresent(int row, int column)
{
    for (int i = 0; i < entities.size(); i++) {
        if (entities.at(i)->getPosition() == QVector2D(row, column))
        {
            return true;
        }
    }
    return false;
}

void Core::timerEvent(QTimerEvent *)
{
    processNextStep();
    exportWorldPositions();
}
