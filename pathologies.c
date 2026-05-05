#include "structures.h"

//Tableau des pathologies

Pathologie pathologies[] = {
    {1, {DETARTREUSE, AUCUN}},              // Caries
    {2, {MIROIR, PINCE}},          // Aphtes
    {1, {PINCE, AUCUN}},              // Parodontite
    {1, {ECARTEUR, DETARTREUSE}},          // Gingivite
    {2, {SONDE, SERINGUE}},          // Abcès
    {2, {ECARTEUR, FRAISE}}           // Malocclusion
};
