#include "structures.h"
#include <stdlib.h>
#include <time.h>

//Tableau des pathologies

Pathologie pathologies[] = {
    {1, {PINCE, AUCUN}},              // Caries
    {2, {ECARTEUR, PINCE}},          // Aphtes
    {1, {PINCE, AUCUN}},              // Parodontite
    {1, {ECARTEUR, AUCUN}},          // Gingivite
    {2, {PINCE, ECARTEUR}},          // Abcès
    {2, {ECARTEUR, PINCE}}           // Malocclusion
};
