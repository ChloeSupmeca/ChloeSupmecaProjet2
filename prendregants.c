#include "projet.h"

void Gants_Dentiste(Dentiste gants){ // ici le dentiste prend des gants
    Gants objet;
    if(gants.g.gloves==0){
        if(objet=='P' || objet=='p'){
            printf("Voulez vous prendre des gants ? \n");
           scanf("%d", &gants.g.gloves);
           if(gants.g.gloves==0){
               printf("Vous n'avez pas pris de gants ! \n");
           }
           else if(gants.g.gloves==1){
                printf("Vous avez pris de gants ! \n");
           }
           else{
                printf("Erreur de saisie ! \n");
                   exit(10);
           }
        }
    }
    else{
        printf("Vous ne pouvez plus prendre de gants ! \n");
    }
    
}
