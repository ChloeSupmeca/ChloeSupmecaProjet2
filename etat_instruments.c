#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define LONGUEUR 8
#define HAUTEUR 9
#define N_FAUTEUILS 4


void Etat_instrument(Pathologie etat){
    int propre=0;
    int sale=1;
    if(etat.instruments[2]==sale){
        printf("Vos instruments sont sales ! \n");
    }
    if(etat.instruments[2]==propre){
        printf("Vos instruments sont propres ! \n");
    }
}
