#include "structures.h"

void deplacement_Dentiste(Dentiste personnage){
    int deplacement;
    
    scanf("%c" &deplacement);
    if(deplacement=='Q'|| deplacement=='q'){
        personnage.p.x=-1;
    }
    if(deplacement=='D'|| deplacement=='d'){
        personnage.p.x=1;
    }
    if(deplacement=='Z'|| deplacement=='z'){
        personnage.p.y=1;
    }
    if(deplacement=='S'|| deplacement=='s'){
        personnage.p.y=-1;
    }
    else{
        printf("erreur !\n");
        exit(20);
    }

}
