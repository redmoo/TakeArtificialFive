#include "core.h"

#include <QDebug>

Core::Core(QObject *parent)
    : QObject(parent)
{}

void Core::initialize(int w_height, int w_width, int entities, int generations, int steps, int speed)
{
    world_height = w_height;
    world_width = w_width;

    number_of_entities = entities;
    number_of_generations = generations;
    steps_per_generation = steps;
    speed_ms = speed;


}
