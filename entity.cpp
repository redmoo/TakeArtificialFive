#include "entity.h"

Entity::Entity(int instr, int pat, QVector2D pos, QObject *parent)
    : status(entity_status::IDLE)
    , beat_counter(0)
    , instrument(instr)
    , patch(pat)
    , position(pos)
    , QObject(parent)
{
}

void Entity::addGene(Gene gene)
{
    genes.append(gene);
}
