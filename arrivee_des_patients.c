#include"structures_projet_dentiste.h"
/* ===================== ARRIVEE DES PATIENTS ===================== */

void faire_arriver_patient(Partie* p) {
    if (p == NULL) return;

int idx = -1;
for (int i = 0; i < N_FAUTEUILS; i++) {
if (!p->patients[i].occupe_fauteuil) { idx = i; break; }
}
if (idx == -1) return;

Patient* pat = &p->patients[idx];
memset(pat, 0, sizeof(Patient));
pat->occupe_fauteuil = 1;
pat->p.x = 7;
pat->p.y = 4 + idx;
pat->type_patho = rand() % NB_PATHOLOGIES;
pat->patho = p->pathologies_data[pat->type_patho];
pat->patience_max = 15 + rand() % 20; /* entre 15 et 34 tours */
pat->patience = pat->patience_max;
pat->plateau.nb_pose = 0;
pat->plateau.sale = 0;
pat->plateau_pose = 1; /* le plateau est toujours a cote du fauteuil */
pat->gratuit = 0;

/* Si un plateau sale est encore present (ancien patient), le nouveau est mecontent -> gratuit */
/* On verifie si le plateau en x=6,y=4+idx est "sale" (ancien contenu) */
/* Ici on gere ca via le flag gratuit */

printf("\n[!] Un patient arrive en fauteuil %d ! Pathologie : %s\n",
idx + 1, noms_pathologies[pat->type_patho]);
printf(" Instruments necessaires : ");
for (int i = 0; i < pat->patho.nb_instruments; i++) {
printf("%s ", noms_instruments[pat->patho.instruments[i]]);
}
printf("\n");
}

/* Verifie les arrivees aleatoires (environ toutes les 8-12 actions) */
void gerer_arrivees(Partie* p) {
    if (p == NULL) return;
/* 1 chance sur 8 par tour qu'un patient arrive si fauteuil libre */
int fauteuil_libre = 0;
for (int i = 0; i < N_FAUTEUILS; i++)
if (!p->patients[i].occupe_fauteuil) { fauteuil_libre = 1; break; }

if (fauteuil_libre && (rand() % 8 == 0)) {
faire_arriver_patient(p);
}
}
