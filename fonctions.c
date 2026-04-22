#include "projet.h"

void Objet_Dentiste(Dentiste objet){ // ici le dentiste prend un objet
    if(objet.ouinon==0){
        printf("Quel objet voulez-vous prendre ? \n");
        scanf("%d", &objet.ouinon);
        if(objet.ouinon==3){
            printf("Vous avez pris la pince ! \n");
        }
        else if(objet.ouinon==2){
            printf("Vous avez pris les ecarteurs ! \n");
        }
        else if(objet.ouinon==0){
            printf("Vous n'avez pris aucun objet ! \n");
          }
        else{
            printf("Erreur de saisie ! \n");
        }
    }
    else{
        printf("Vous ne pouvez plus prendre d'objet ! \n");
    }
}
