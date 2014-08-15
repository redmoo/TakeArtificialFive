#include "core.h"

#include "random_generator.h"

#include <QDebug>
#include <iostream>
#include <ctime>
#include <algorithm>

const int Core::instruments[5] = { 0, 52, 0, 35, 81 }; // 0=piano, 52=choir aahs, 48=strings, 35=fretless bass, 74=pan flute

Core::Core(QObject *parent)
    : state(simulation_state::IDLE)
    , QObject(parent)
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

    for (int i = 0; i < number_of_entities; i++) {

        Entity *ent = new Entity(instruments[i % 5], i >= 10 ? i+1 : i, generateInitialPosition());
        assignGenes(number_of_genes, ent);
        entities.append(ent);

        midi_engine.setPatch(ent->patch, ent->instrument);

    }

    step_counter = 0;
    resumeSimulation();

}

void Core::pauseSimulation()
{
    if (state == simulation_state::RUNNING) {
        state = simulation_state::PAUSED;
        killTimer(timer_id);
    }
}

void Core::resumeSimulation()
{
    if (state != simulation_state::RUNNING) {
        state = simulation_state::RUNNING;
        timer_id = startTimer(speed_ms);
    }
}

void Core::processNextStep()
{
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        if (entity->state == entity_state::PLAYING && entity->beat_counter == 0) {
            midi_engine.stopNote(entity->current_tone, entity->patch);
            entity->state = entity_state::IDLE;
        }
        else if (entity->state == entity_state::PLAYING && entity->beat_counter > 0) {
            continue;
        }
        else if(entity->state == entity_state::IDLE && entity->beat_counter > 0) {
            qDebug() << "ERROR: IDLE && beat_counter > 0."; // TEST
        }


        for (int g = 0; g < entity->genes.size(); g++) {
            Gene *gene = entity->genes[g];
            QVector<Entity*> neighbours = getEntityNeighbours(entity, gene->getPerceptionRadius());

            if (gene->trigger(neighbours)) {

                QVector3D new_tone = gene->generateTone(neighbours);
                entity->current_tone = new_tone.x();
                entity->loudness = new_tone.y();
                entity->beat_counter = new_tone.z();
                entity->position_delta = gene->generateMovementDelta();
                break;
            }
        }
    }

    // loop ko zaigras ton in dekrementiras play counter (vsem...) in premaknes entitete na nove queued up pozicije + status
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        // PLAY and MOVE
        if (entity->state == entity_state::IDLE && entity->beat_counter > 0) {
            entity->track.append(entity->current_tone);

            // TEMP!
            //int loudness = entity->loudness;
            //if (entity->current_tone == 68) loudness = 40;
            //else if (entity->current_tone == 54) loudness = 80;

            midi_engine.playNote(entity->current_tone, entity->patch, entity->loudness); // nared da lahko igrajo NIC za x dob
            entity->state = entity_state::PLAYING;
            entity->beat_counter--;

            int vertical = (int)(entity->position.x() + entity->position_delta.x());
            int horizontal = (int)(entity->position.y() + entity->position_delta.y());
            entity->position = QVector2D(vertical < 0 ? world_height + vertical : vertical % world_height,
                                         horizontal < 0 ? world_width + horizontal : horizontal % world_width);
        }
        else if (entity->state == entity_state::PLAYING) {
            entity->track.append(midi_state::SUSTAIN);
            entity->beat_counter--;
        }
        else {
            entity->track.append(midi_state::PAUSE);
        }

        // EVALUATE

    }
}

void Core::exportWorldPositions()
{
    QVector<int> world;

    qDebug() << "Step: " << step_counter;
    for (int i = 0; i < world_height; i++) {
        for (int j = 0; j < world_width; j++) {
            int index = entityPresent(i, j);
            if (index != -1) {
                entities.at(index)->state == entity_state::PLAYING
                        ? world.append(entities.at(index)->current_tone)
                        : world.append(-1);
            }
            else {
                world.append(-2);
            }
        }   
    }

    emit worldChanged(world, world_height, world_width);
}

Gene* Core::initializeRandomGene()
{
    int gene_index = RandomGenerator::get()->random(0, 0); // manual update

    switch (gene_index) {
        case 0: return new LonelyGene();
        //case 1: return SeekingGene();
    }
}

void Core::assignGenes(int amount, Entity *ent)
{
    for (int i = 0; i < amount; i++) {
        ent->genes.append(initializeRandomGene());
    }
    std::sort(ent->genes.begin(), ent->genes.end(), [](const Gene* g1, const Gene* g2) {
        return g1->getPriority() > g2->getPriority();
    });
}

QVector2D Core::generateInitialPosition()
{
    return QVector2D(RandomGenerator::get()->random(0, world_height-1),
                     RandomGenerator::get()->random(0, world_width-1));
}

QVector<Entity *> Core::getEntityNeighbours(Entity* parent, int radius)
{
    QVector<Entity *> neighbours;

    for (int i = 0; i < entities.size(); i++) {
        if (entities.at(i) == parent) continue;
        else if (inRadius(parent->position, entities.at(i)->position, radius)) {
            neighbours.append(entities.at(i));
        }
    }

    return neighbours;
}

bool Core::inRadius(QVector2D center, QVector2D neighbour, int radius)
{
    int dh = std::abs(center.x() - neighbour.x());
    dh = dh > world_height/2 ? world_height - dh : dh;
    int dw = std::abs(center.y() - neighbour.y());
    dw = dw > world_width/2 ? world_width - dw : dw;

    return dh <= radius && dw <= radius;
}

int Core::entityPresent(int row, int column)
{
    for (int i = 0; i < entities.size(); i++) {
        if (entities.at(i)->position == QVector2D(row, column))
        {
            return i;
        }
    }
    return -1;
}

void Core::timerEvent(QTimerEvent *)
{
    // nared medoto k handla ta shit... pa next step razdel na podmetode...
    step_counter++;
    processNextStep();
    exportWorldPositions();

    if (step_counter == steps_per_generation) {
        killTimer(timer_id);
        midi_engine.exportTrack(entities, speed_ms, RandomGenerator::get()->getSeed());
    }

}
