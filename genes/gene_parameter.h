#ifndef GENE_PARAMETER_H
#define GENE_PARAMETER_H

class GeneParameter
{

public:
    GeneParameter()
    {}

    GeneParameter(int p_min, int p_max)
        : min(p_min)
        , max(p_max)
    {
        mutate();
    }

    void mutate()
    {
        // boost random
        value = 0;
    }

private:
    int value;
    int min;
    int max;

};

#endif // GENE_PARAMETER_H
