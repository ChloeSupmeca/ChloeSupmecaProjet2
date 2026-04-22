#include "projet.h"

void Gants_Dentiste(Dentiste gants){ // ici le dentiste prend des gants
    if(gants.g.gloves==0){
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
       }
    }
    else{
        printf("Vous ne pouvez plus prendre de gants ! \n");
    }
    
}
