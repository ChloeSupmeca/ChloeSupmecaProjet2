#ifndef UTILS_H
#define UTILS_H

#include "types.h"


int  prix_prestation(Pathologie* patho);
const char* symbole_instrument(Instrument i);
char symboleCase(Case b);


int instrument_utile(Patient* pat, Instrument instr);
int instrument_deja_pose(Patient* pat, Instrument instr);
int plateau_complet(Patient* pat);
int presence_patient(Partie* p, int x, int y);
int plateau_a_patient(Partie* p, int x, int y, int* idx_patient);
int plateau_a_patient_ou_libre(Partie* p, int x, int y, int* idx_patient);


void afficher_barre_patience(Patient* pat);
void affichage(Partie* p);

#endif 
