#include "structures.h"

Dentiste soigner_Patient(Dentist personnage){
    Dentiste soin;
    scanf("%c", &soin);
    if(soin=='K' || soin=='k'){
       printf("Patient(e) guéri(e) !\n");
    }
    else{
        printf("erreur de saisie!\n");
        exit(50);
    }
    return soin;
}
