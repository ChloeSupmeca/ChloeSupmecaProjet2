#inlcude"structures_projet_dentiste.h"

/* ===================== UTILITAIRES ===================== */

/* Calcul du prix d'une prestation (somme des couts des instruments necessaires) */
int prix_prestation(Pathologie* patho) {
 
    return patho->prix;

}

/* Noms courts pour affichage */
const char* symbole_instrument(Instrument i) {
 switch(i) {
 case PINCE:
  return "PIN";
 case ECARTEURS:
  return "ECA";
 case SERINGUE:
  return "SER";
 case MIROIR: 
  return "MIR";
 case SONDE: 
  return "SON";
 case FRAISE:
  return "FRA";
 case DETARTREUSE:
  return "DET";
 default: 
  return "---";
 }
}

/* Retourne 1 si l'instrument est dans la pathologie du patient */
int instrument_utile(Patient* pat, Instrument instr) {
for (int i = 0; i < pat->patho.nb_instruments; i++) {
 if (pat->patho.instruments[i] == instr) {
  return 1;
 }
}
return 0;
}

/* Retourne 1 si l'instrument est deja sur le plateau */
int instrument_deja_pose(Patient* pat, Instrument instr) {
 for (int i = 0; i < pat->plateau.nb_pose; i++) {
  if (pat->plateau.pose[i] == instr){
   return 1;
   }
 }
return 0;
}

/* Retourne 1 si tous les instruments necessaires sont sur le plateau */
int plateau_complet(Patient* pat) {
for (int i = 0; i < pat->patho.nb_instruments; i++) {
 if (!instrument_deja_pose(pat, pat->patho.instruments[i])){
  return 0;
 }
}
return 1;
}

/* Symbole de la case */
char symboleCase(Case b) {
char symboles[] = {' ', 'F', 'T', 'I', 'R', 'B', 'G', 'M'};
return symboles[(int)b];
}

/* Verifie si un patient est sur une case */
int presence_patient(Partie* p, int x, int y) {
    if (p == NULL){
     return 0;
    }
for (int i = 0; i < N_FAUTEUILS; i++) {
if (p->patients[i].occupe_fauteuil &&
p->patients[i].p.x == x && p->patients[i].p.y == y) {
return i + 1; /* retourne l'index+1 */
}
}
return 0;
}

/* Fauteuil associe a chaque plateau */
/* Plateaux en x=6, fauteuils en x=7, meme y */
int plateau_a_patient(Partie* p, int x, int y, int* idx_patient) {
    if (p == NULL){
     return 0;
    }
if (x == 6 && y >= 4 && y <= 7) {
 for (int i = 0; i < N_FAUTEUILS; i++) {
  if (p->patients[i].occupe_fauteuil &&
 p->patients[i].p.y == y) {
   *idx_patient = i;
  return 1;
    }
   }
  }
return 0;
}

int plateau_a_patient_ou_libre(Partie* p, int x, int y, int* idx_patient) {
    if (p == NULL){
     return 0;
    }
    if (x == 6 && y >= 4 && y <= 7) {
        *idx_patient = y - 4;
        return 1;
    }
    return 0;
}
