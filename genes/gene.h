#ifndef GENE_H
#define GENE_H

#include "gene_parameter.h"

class Gene // a rabm da je to qobject? pomoje ne...?
{

public:
    explicit Gene();

    // todo: iz entitet znotraj radija doloci ali se sprozi in kaksen ton se zaigra
    // a ma entiteta metodo in prek radija gena genu posreduje samo tiste znotraj?
    void mutateParameters(double mutation_rate); //nemorm dat virtual k pol ne pozre vektor

protected:
    GeneParameter priority;
    GeneParameter perception_radius;

};

#endif // GENE_H
