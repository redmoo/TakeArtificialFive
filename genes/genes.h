#ifndef GENES_H
#define GENES_H

#include "gene.h"

class LonelyGene : public Gene
{

public:
    explicit LonelyGene();

    void mutateParameters();

};

class SeekingGene : public Gene
{

public:
    explicit SeekingGene();

    void mutateParameters();
};

#endif // GENES_H
