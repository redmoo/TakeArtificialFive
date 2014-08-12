#include "gene.h"

Gene::Gene()
{
    priority.initialize(0, 7); // a bi pac kle raj spustu to pa se lepo mutate klice za vse... al se tko najprej sami pa pol mutiras k je treba, ceprv je isti kurac?
    perception_radius.initialize(1, 5);
}
