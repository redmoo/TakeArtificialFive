#ifndef GENE_PARAMETER_H
#define GENE_PARAMETER_H

#include "../random_generator.h"

class GeneParameter
{

public:
    GeneParameter()
    {}

    /*GeneParameter(int p_min, int p_max) // to pol nemorm ponucat al kako?
        : min(p_min)
        , max(p_max)
    {
        mutate();
    }*/ // ZAKVA TO NE POZRE????

    void initialize(int p_min, int p_max)
    {
        min = p_min;
        max = p_max;
        mutate();
    }

    void mutate()
    {
        value = RandomGenerator::get()->random(min, max);
    }

    int getValue() const { return value; }

private:
    int value;
    int min;
    int max;

};

#endif // GENE_PARAMETER_H
