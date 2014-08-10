#ifndef CORE_H
#define CORE_H

#include "entity.h"

#include <QObject>

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = 0);

public:
    void initialize(int w_height, int w_width, int entities, int generations, int steps, int speed);

private:


private:
    int world_height;
    int world_width;

    int number_of_entities;
    int number_of_generations;
    int steps_per_generation;
    int speed_ms;

};

#endif // CORE_H
