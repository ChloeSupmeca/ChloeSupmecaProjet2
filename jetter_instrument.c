#include "structures.h"

void jetter_Instrument(Dentiste personnage){
    Dentiste objet;
    
    scanf("%c", &objet.hand);
    
    if(objet.hand=='W' || objet.hand=='w'){
        printf("Vous avez jetté un instrument ! \n");
    }
    else{
        printf("Erreur !\n");
        exit(40);
    }

}
