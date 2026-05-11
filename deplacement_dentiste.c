#include "structures.h"

void deplacement_Dentiste(Partie *p, char direction){
    int newX = p->dentiste.p.x;
    int newY = p->dentiste.p.y;
    
    switch (direction) {
        case '5': 
            newY--; 
            break;  // haut
        case '2': 
            newY++; 
            break;  // bas
        case '1':
            newX--; 
            break;  // gauche
        case '3': 
            newX++;
            break;  // droite
        default: 
            return;
    }
    
    if (newX < 0 || newX >= LONGUEUR || newY < 0 || newY >= HAUTEUR)
        return;
        
    if(p->grille[newX][newY] == SOL){
        p->dentiste.p.x = newX;
        p->dentiste.p.y = newY;
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
