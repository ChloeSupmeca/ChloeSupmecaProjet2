#include "structures.h"
#include <stdlib.h>

void genererPatient(Patient *p) {
    p->occupe_fauteuil = 1;
    p->patience = rand() % 30 + 20;  
    p->treated = 0;

    int r = rand() % 6;  
    p->patho = pathologies[r];
}
