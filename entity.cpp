#include "entity.h"

Entity::Entity(int instr, int pat, QVector2D pos, QObject *parent)
    : state(entity_state::IDLE)
    , beat_counter(0)
    , instrument(instr)
    , patch(pat)
    , position(pos)
    , QObject(parent)
{
}

int Entity::getInstrument()
{
    return instrument;
}

int Entity::getPatch()
{
    return patch;
}

QVector<int> Entity::getTrack()
{
    return track;
}
