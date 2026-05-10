#include "structures.h"

Dentiste jetter_Instrument(Dentiste personnage){
    Dentiste objet;
    
    scanf("%c", &objet.mains);
    
    if(objet.mains=='W' || objet.mains=='w'){
        printf("Vous avez jetté un instrument ! \n");
    }
    else{
        printf("Erreur !\n");
        exit(40);
    }
    return objet;
}
