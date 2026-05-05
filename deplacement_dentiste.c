#include "structures.h"

void deplacement_Dentiste(Dentist personnage){
    int deplacement;
    
    if(deplacement=='Q'){
        personnage.p.x=-1;
    }
    if(deplacement=='D'){
        personnage.p.x=1;
    }
    if(deplacement=='Z'){
        personnage.p.y=1;
    }
    if(deplacement=='S'){
        personnage.p.y=-1;
    }

}
