#ifndef GENE_H
#define GENE_H

#include "gene_parameter.h"
#include "../entity.h"

#include <QVector2D>

class Entity;

class Gene
{

public:
    explicit Gene();

    // todo: iz entitet znotraj radija doloci ali se sprozi in kaksen ton se zaigra
    // a ma entiteta metodo in prek radija gena genu posreduje samo tiste znotraj?
    virtual bool trigger(QVector<Entity*> neighbours) = 0;
    virtual QVector3D generateTone(QVector<Entity*> neighbours) = 0; // QVector3D(tone, loudness, duration)
    virtual QVector2D generateMovementDelta() = 0; // kasnej bo glede na sosede & sebe

    virtual void mutateParameters(double mutation_rate) = 0;
    virtual void resetGene() = 0;

    int getPriority() const;
    int getPerceptionRadius() const;

protected:
    void mutateBase(double mutation_rate);

protected:
    GeneParameter priority;
    GeneParameter perception_radius;

};

#endif // GENE_H
