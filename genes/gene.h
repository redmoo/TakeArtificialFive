#ifndef GENE_H
#define GENE_H

#include "gene_parameter.h"

#include <QObject>

class Gene : public QObject
{
    Q_OBJECT

public:
    explicit Gene(QObject *parent = 0);

    // todo: iz entitet znotraj radija doloci ali se sprozi in kaksen ton se zaigra
    // a ma entiteta metodo in prek radija gena genu posreduje samo tiste znotraj?
    void mutateParameters(double mutation_rate);

private:
    GeneParameter priority;
    GeneParameter perception_radius;

};

#endif // GENE_H
