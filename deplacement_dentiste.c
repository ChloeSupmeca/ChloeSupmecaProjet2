#include "structures.h"

void deplacement_Dentiste(Partie *p, int dx, int dy){
   
    int newX = p->dentiste.p.x  + dx;
    int newY = p->dentiste.p.y + dy;

    if(newX >=0 && newY <LONGUEUR && newY>=0 && p->grille[newX][newY]== SOL){
        p->dentiste.p.x=newX;
        p->dentiste.p.y=newY;
    }
}
    //FONCTION POUR INTERAGIR :

    /*int deplacement;
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
*/
