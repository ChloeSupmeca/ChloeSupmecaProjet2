#ifndef ACTIONS_H
#define ACTIONS_H

/* Toutes les actions realisables par le dentidtse. Chaque action verifie les preconditions avant d'agir, si elles ne sont pas respectees rien ne change */

#include "types.h"

void action_prendre_gants(Partie* p);
void action_jeter_gants(Partie* p);
void action_prendre_instrument(Partie* p);
void action_deposer_plateau(Partie* p);
void action_soigner(Partie* p);
void action_vider_plateau_biologique(Partie* p);
void action_jeter_recyclage(Partie* p);


void action_e(Partie* p);


void deplacer(Partie* p, int dx, int dy);

#endif 
