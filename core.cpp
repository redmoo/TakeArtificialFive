#include "core.h"

#include "random_generator.h"

#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <omp.h>

const int Core::instruments[7] = { 0, 0, 0, 35, 0, 0, 46}; // 0=piano, 52=choir aahs, 48=strings, 35=fretless bass, 74=pan flute

Core::Core(QObject *parent)
    : state(simulation_state::IDLE)
    , QObject(parent)
{}

void Core::initialize(time_t seed, int w_height, int w_width, int nr_of_entities, int nr_of_genes, QString g_string, int generations, int steps, int speed)
{
    world_height = w_height;
    world_width = w_width;

    number_of_entities = nr_of_entities;
    number_of_genes = nr_of_genes;
    total_genes = 3;
    number_of_generations = generations;
    steps_per_generation = steps;
    speed_ms = current_speed = speed;

    // SETUP EVERYTHING ...
    if (seed != 0) {
        RandomGenerator::get()->setSeed(seed);
    }
    emit seedChanged(RandomGenerator::get()->getSeed());
    emit newConsoleMessage(QString("Seed: %1 \n").arg(QString::number(RandomGenerator::get()->getSeed())));

    bool gene_string_valid = geneStringValidation(g_string);

    for (int i = 0; i < number_of_entities; i++) {

        Entity *ent = new Entity(instruments[RandomGenerator::get()->random(0, 6)], i >= 10 ? i+1 : i, generateInitialPosition());
        ent->current_tone = midi_state::PAUSE;

        QString gene_substring = gene_string_valid ? g_string.mid(number_of_genes * i, number_of_genes) : QString();

        assignGenes(number_of_genes, ent, gene_substring);
        entities.append(ent);

        midi_engine.setPatch(ent->patch, ent->instrument);

    }

    transposition = transposition_queue = 0;
    generation_counter = 1; // updejti .. a gremo od 0 al od 1...
    step_counter = 0;
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
        timer_id = startTimer(current_speed);
    }
}

void Core::processNextStep()
{
    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        if (entity->state == entity_state::PLAYING && entity->beat_counter == 0) {
            if(current_speed != 0 && !fast_forward)
                midi_engine.stopNote(entity->current_tone + transposition, entity->patch);
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
    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        // PLAY and MOVE
        if (entity->state == entity_state::IDLE && entity->beat_counter > 0) {
            entity->track.append(entity->current_tone);

            if (entity->current_tone != midi_state::PAUSE && current_speed != 0 && !fast_forward)
                midi_engine.playNote(entity->current_tone + transposition, entity->patch, entity->loudness); // nared da lahko igrajo NIC za x dob
            entity->state = entity_state::PLAYING;
            entity->beat_counter--;

            int vertical = (int)(entity->position.x() + entity->position_delta.x());
            int horizontal = (int)(entity->position.y() + entity->position_delta.y());
            entity->position = QVector2D(vertical < 0 ? world_height + vertical : vertical % world_height,
                                         horizontal < 0 ? world_width + horizontal : horizontal % world_width);
        }
        else if (entity->state == entity_state::PLAYING) {
            if (entity->current_tone != midi_state::PAUSE)
                entity->track.append(midi_state::SUSTAIN);
            else
                entity->track.append(midi_state::PAUSE);
            entity->beat_counter--;
        }
        else {
            entity->track.append(midi_state::PAUSE);
        }

    }
}

void Core::evaluateEntities()
{
    // consonance and activity

    QVector<int> consonant_intervals = {0, 3, 4, 5, 7, 8, 9, 12};

    QVector<int> consonance_count(entities.size(), 0); // ratio glede na zaigrano + disonanca konsonanca.. lahk tweakas
    QVector<int> intervals_compared(entities.size(), 0);
    QVector<int> quiet_count(entities.size(), 0);

    QVector<int> current_beat(entities.size(), midi_state::PAUSE);

    #pragma omp parallel for
    for (int beat = 0; beat < steps_per_generation; beat++) {

        for (int i = 0; i < entities.size(); i++) {
            int tone = entities.at(i)->track.at(beat);
            if (tone != midi_state::SUSTAIN) current_beat[i] = tone;
            if (tone == midi_state::PAUSE) quiet_count[i]++;
        }

        for (int e = 0; e < entities.size() - 1; e++) {
            int entity_tone = current_beat.at(e);

            for (int p = e + 1; p < entities.size(); p++) {
                int pair_tone = current_beat.at(p);

                if(entity_tone != midi_state::PAUSE && pair_tone == midi_state::PAUSE) {
                    consonance_count[e]++;
                    intervals_compared[e]++;
                    continue;
                }
                else if (entity_tone == midi_state::PAUSE && pair_tone != midi_state::PAUSE) {
                    consonance_count[p]++;
                    intervals_compared[p]++;
                    continue;
                }
                else if (entity_tone == midi_state::PAUSE && pair_tone == midi_state::PAUSE) {
                    continue;
                }

                int interval = std::abs(entity_tone - pair_tone);
                if (consonant_intervals.contains(interval % 12)) { // a ni skor bols ce je sm 12 oz nastavs radius!
                    consonance_count[e]++;
                    consonance_count[p]++;
                }

                intervals_compared[e]++;
                intervals_compared[p]++;

            }

        }
    }

    // tone & rhythm

    QVector<double> tone_scores(entities.size(), 0);
    QVector<double> rhythm_scores(entities.size(), 0);

    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);
        QMap<int, int> tone_count;
        QMap<int, int> rhythm_count;

        int tones_played = 0;
        int tones_played_full_duration = 0;

        int duration_count = 0;

        for (int beat = 0; beat < steps_per_generation; beat++) {
            int tone = entity->track.at(beat);

            // tone
            if (tone != midi_state::PAUSE && tone != midi_state::SUSTAIN) {
                tones_played++;

                if (!tone_count.contains(tone)) tone_count.insert(tone, 1);
                else tone_count[tone]++;
            }

            // rhythm (ignoriramo zadnjo noto, ki je lahko odrezana zaradi stevila stepov)
            if (tone != midi_state::SUSTAIN && duration_count > 0) {
                if (!rhythm_count.contains(duration_count)) rhythm_count.insert(duration_count, 1);
                else rhythm_count[duration_count]++;
                duration_count = 0;
                tones_played_full_duration++;
            }

            if (tone != midi_state::PAUSE) duration_count++;
        }

        // tone
        double t_score = 1.0; // a dam 0 ce ni nobenga tona igral?
        // napis u diplomo da je hujs ce je 00000000001 kot 0001

        foreach (int t_value, tone_count) {
            double percentage = (double)t_value / (double)tones_played;

            if (tone_binary && percentage > tone_max_similarity) {
                t_score = 0;
                break;
            }
            t_score -= percentage > tone_max_similarity ? percentage - tone_max_similarity : 0.0;
        }
        tone_scores[e] = t_score;

        // rhythm
        double r_score = 1.0;

        foreach (int r_value, rhythm_count) {
            double percentage = (double)r_value / (double)tones_played_full_duration;

            if (rhythm_binary && percentage > rhythm_max_similarity) {
                r_score = 0;
                break;
            }
            r_score -= percentage > rhythm_max_similarity ? percentage - rhythm_max_similarity : 0.0;
        }
        rhythm_scores[e] = r_score;
    }

    // scoring

    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        entity->consonant_score = intervals_compared.at(e) > 0 ? (double)consonance_count.at(e) / (double)intervals_compared.at(e) : 0;
        entity->disonant_score = intervals_compared.at(e) > 0 ? 1 - entity->consonant_score : 0;
        entity->activity_score = (double)(steps_per_generation - quiet_count.at(e)) / (double)steps_per_generation;
        entity->inactivity_score = 1 -  entity->activity_score; // obrn to dvoje okol

        entity->tone_score = tone_scores.at(e);
        entity->rhythm_score = rhythm_scores.at(e);

        entity->score =
                consonance_coeff * entity->consonant_score +
                disonance_coeff * entity->disonant_score +
                activity_coeff * entity->activity_score +
                inactivity_coeff * entity->inactivity_score +
                tone_coeff * entity->tone_score +
                rhythm_coeff * entity->rhythm_score +
                neutral_coeff;
    }

}

void Core::mutateEntities()
{
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        if (entity->score < fitness_cutoff) {
            double mutation_rate = double(1 - entity->score) * mutation_factor;

            for (int g = 0; g < entity->genes.size(); g++) {
                entity->genes.at(g)->mutateParameters(mutation_rate);
            }

            // TEST - mutate initial position; Test2 = gene has own starting position
            if (mutate_initial_position && RandomGenerator::get()->random01() <= mutation_rate) {
                entity->initial_position = generateInitialPosition();
            }

            entity->mutation_rate = mutation_rate;
        }
        else {
            entity->mutation_rate = 0;
        }
    }

}

void Core::displayScores()
{
    emit newConsoleMessage(QString("FC=%1; M=%2;")
                           .arg(QString::number(fitness_cutoff, 'f', 2))
                           .arg(QString::number(mutation_factor, 'f', 2)));

    emit newConsoleMessage(QString("C=%1; D=%2; A=%3; I=%4; T=%5; R=%6; N=%7 \n")
                           .arg(QString::number(consonance_coeff, 'f', 2))
                           .arg(QString::number(disonance_coeff, 'f', 2))
                           .arg(QString::number(activity_coeff, 'f', 2))
                           .arg(QString::number(inactivity_coeff, 'f', 2))
                           .arg(QString::number(tone_coeff, 'f', 2))
                           .arg(QString::number(rhythm_coeff, 'f', 2))
                           .arg(QString::number(neutral_coeff, 'f', 2)));


    emit newConsoleMessage(QString("        [Total]     [C]       [D]         [A]        [I]        [T]        [R]"));

    for (int e = 0; e < entities.size(); e++) {

        QString message = QString("E%1:   %2        %3     %4      %5      %6     %7     %8     %9")
                .arg(QString::number(e))
                .arg(QString::number(entities.at(e)->score, 'f', 2))
                .arg(QString::number(entities.at(e)->consonant_score, 'f', 2))
                .arg(QString::number(entities.at(e)->disonant_score, 'f', 2))
                .arg(QString::number(entities.at(e)->activity_score, 'f', 2))
                .arg(QString::number(entities.at(e)->inactivity_score, 'f', 2))
                .arg(QString::number(entities.at(e)->tone_score, 'f', 2))
                .arg(QString::number(entities.at(e)->rhythm_score, 'f', 2))
                .arg(entities.at(e)->mutation_rate > 0 ? QString("M=%1").arg(QString::number(entities.at(e)->mutation_rate, 'f', 2)) : "");
        //qDebug() << message;
        emit newConsoleMessage(message);
    }

    qDebug() << "";
    emit newConsoleMessage(QString("_________________________________________________________\n"));
}

void Core::resetEntities()
{
    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);

        midi_engine.stopNote(entity->current_tone, entity->patch);

        entity->track.clear();
        //entity->current_tone = midi_state::PAUSE; // ZIHR DODA NEKAJ ZANIMIVEGA CE NE CLEARAMO TEGA
        entity->position = entity->initial_position;
        entity->beat_counter = 0;
        entity->mutation_rate = 0;
        entity->state = entity_state::IDLE;

        for (int g = 0; g < entity->genes.size(); g++) {
            entity->genes.at(g)->resetGene();
        }
    }
}

void Core::stopCurrentTones()
{
    #pragma omp parallel for
    for (int e = 0; e < entities.size(); e++) {
        Entity *entity = entities.at(e);
        midi_engine.stopNote(entity->current_tone, entity->patch);
    }
}

void Core::assembleCurrentTrack()
{
    current_track.clear();
    for (int e = 0; e < entities.size(); e++) {
        current_track.append(entities.at(e)->track);
    }
}

bool Core::geneStringValidation(QString g_string)
{
    g_string = g_string.trimmed();

    if (g_string.length() != number_of_entities * number_of_genes)
        return false;

    for (int i = 0; i < g_string.length(); i++) {
        int gene_index = g_string.at(i).digitValue();
        if (gene_index < 0 || gene_index >= total_genes)
            return false;
    }
    return true;
}

void Core::exportWorldPositions()
{
    QVector<int> world;

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
    emit simulationCountChanged(generation_counter, step_counter);
}

void Core::setCurrentSpeed(int speed)
{
    current_speed = speed;
    killTimer(timer_id);
    if (current_speed == 0) stopCurrentTones(); // to najbrs ne prime...
    timer_id = startTimer(current_speed);
}

void Core::updateFitnessCutoff(double cutoff)
{
    fitness_cutoff = cutoff;
}

void Core::updateMutationFactor(double mutation)
{
    mutation_factor = mutation;
}

void Core::toggleInitialPositionMutation(bool mutate)
{
    mutate_initial_position = mutate;
}

void Core::updateFitness(double consonance, double disonance,
                         double activity, double inactivity,
                         double tonality, double tonality_max, bool tonality_bin,
                         double rhythm, double rhythm_max, bool rhythm_bin)
{
    consonance_coeff = consonance;
    disonance_coeff = disonance;

    activity_coeff = activity;
    inactivity_coeff = inactivity;

    tone_coeff = tonality;
    tone_max_similarity = tonality_max;
    tone_binary = tonality_bin;

    rhythm_coeff = rhythm;
    rhythm_max_similarity = rhythm_max;
    rhythm_binary = rhythm_bin;

    neutral_coeff = 1.0 - consonance_coeff - disonance_coeff - activity_coeff - inactivity_coeff - tone_coeff - rhythm_coeff;
}

void Core::toggleGenerationExport(bool export_current)
{
    export_current_generation = export_current;
}

void Core::toggleFastForward(bool ff)
{
    if (!ff && generation_counter > 1) displayScores();
    stopCurrentTones();
    fast_forward = ff;
}

void Core::setTransposition(int t)
{
    transposition_queue = t;
}

Gene* Core::initializeRandomGene(int index)
{
    int gene_index = index == -1 ? RandomGenerator::get()->random(0, total_genes - 1) : index; // manual update

    // nared checkboxe za kateri geni so v poolu
    switch (gene_index) {
        case 0: return new LonelyGene();
        case 1: return new ChordGene(steps_per_generation);
        case 2: return new QuietGene();
    }
}

void Core::assignGenes(int amount, Entity *ent, QString g_string)
{
    for (int i = 0; i < amount; i++) {
        ent->genes.append(initializeRandomGene(g_string.length() > 0 ? g_string.at(i).digitValue() : -1));
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
    dh = dh > (double)world_height/2 ? world_height - dh : dh;
    int dw = std::abs(center.y() - neighbour.y());
    dw = dw > (double)world_width/2 ? world_width - dw : dw;

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
    int FF_counter = 0;

    while (FF_counter < 1 || fast_forward) {
        // nared medoto k handla ta shit... pa next step razdel na podmetode...
        step_counter++;
        processNextStep();
        exportWorldPositions();

        if ((generation_counter < number_of_generations || number_of_generations == 0) && step_counter == steps_per_generation) {

            //assembleCurrentTrack(); // a to sploh rabm
            evaluateEntities();
            mutateEntities();
            if (!fast_forward) displayScores();

            if (export_current_generation)
                midi_engine.exportTrack(entities, current_speed + 10, RandomGenerator::get()->getSeed(), generation_counter, world_height, world_width, steps_per_generation, transposition);

            resetEntities();
            step_counter = 0;
            generation_counter++;

            transposition = transposition_queue;
        }
        else if (generation_counter == number_of_generations && step_counter == steps_per_generation) {
            killTimer(timer_id);
            midi_engine.close();
            midi_engine.exportTrack(entities, speed_ms, RandomGenerator::get()->getSeed(), generation_counter, world_height, world_width, steps_per_generation, transposition); // nared metodo k vrne ime fajla in passas ime
        }

        FF_counter++;
        if (FF_counter % steps_per_generation == 0) QCoreApplication::processEvents();
    }
}
