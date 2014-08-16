#include "gene.h"
#include <QDebug>

Gene::Gene()
{
    priority.initialize(0, 7); // a bi pac kle raj spustu to pa se lepo mutate klice za vse... al se tko najprej sami pa pol mutiras k je treba, ceprv je isti kurac?
    perception_radius.initialize(1, 5);
}

int Gene::getPriority() const
{
    return priority.getValue();
}

int Gene::getPerceptionRadius() const
{
    return perception_radius.getValue();
}

void Gene::mutateBase(double mutation_rate)
{
    if (RandomGenerator::get()->random01() <= mutation_rate) priority.mutate();
    if (RandomGenerator::get()->random01() <= mutation_rate) perception_radius.mutate();
}
