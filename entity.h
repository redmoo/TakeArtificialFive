#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QVector>
#include <QVector2D>

namespace entity_status
{
    const int IDLE = 1000;
    const int PLAYING = 1001;
}

class Entity : public QObject
{
    Q_OBJECT

public:
    explicit Entity(QObject *parent = 0);

private:
    int status;
    int beat_counter;

    int instrument;
    QVector<int> track;

    int current_tone;
    //int last_played_tone; // to lahko dobimo iz metode + track record

    QVector2D position;
    QVector2D position_queue;
    double score;

    // gene list

};

#endif // ENTITY_H
