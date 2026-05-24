#ifndef PARTIE_H
#define PARTIE_H

#include "types.h"

/* Initialisation */
void init_grille(Partie* p);
void nouvelle_partie(Partie* p);

/*  Chargement fichiers */
Instrument instrument_depuis_nom(char* nom);
void charger_couts(Partie* p);
void charger_pathologies(Partie* p);

/*  Patients  */
void faire_arriver_patient(Partie* p);
void gerer_arrivees(Partie* p);
void gerer_patience(Partie* p);

/*  Sauvegarde  */
void sauvegarder(Partie* p);
int  charger_sauvegarde(Partie* p);
int  fichier_sauvegarde_existe(void);

/*  Scores  */
void enregistrer_score(Partie* p);

#endif /* PARTIE_H */
