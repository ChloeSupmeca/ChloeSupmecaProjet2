#include "projet.h"

void Gants_Dentiste(Dentiste prendregants){ // ici le dentiste prend des gants
    Gants objet;
    if(prendregants.g.gloves==0){
        if(objet=='G' || objet=='g'){
            printf("Voulez vous prendre des gants ? \n");
           scanf("%d", &prendregants.g.gant);
           if(prendregants.g.gant==0){
               printf("Vous n'avez pas pris de gants ! \n");
           }
           else if(prendregants.g.gant==1){
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
