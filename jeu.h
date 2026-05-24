/* Interface de la boucle principale et du menu de demarrage. La boucle 
principale du jeu lit les commandes du joueur , met a jour l'etat du jeu, 
gere les arrivees et la patience, se termine qund p->partie vaut 1.*/

#ifndef JEU_H
#define JEU_H

#include "types.h"

void afficher_menu_principal(void);
void boucle_jeu(Partie* p);

#endif 
