#include"structures_projet_dentiste.h"
/* ===================== AFFICHAGE ===================== */

void afficher_barre_patience(Patient* pat) {
int pct = (pat->patience * 10) / (pat->patience_max > 0 ? pat->patience_max : 1);


printf("[");
for (int k = 0; k < 10; k++) printf(k < pct ? "#" : ".");
printf("] %2d", pat->patience);

}

void affichage(Partie* p) {
    if (p == NULL) return;
printf("\033[2J\033[H"); /* Clear screen */

printf("╔══════════════════════════════════════════════════════════════════╗\n");
printf("║ CABINET DENTAIRE ║\n");
printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

/* Legende */
printf("D=Dentiste F=Fauteuil T=Plateau I=Instruments G=Gants\n");
printf("R=Recyclage B=Biologique M=Mur P=Patient\n\n");

/* Grille */
printf(" ");
for (int x = 0; x < LONGUEUR; x++) printf("%d ", x);
printf("\n");

for (int y = 0; y < HAUTEUR; y++) {
printf("%d ", y);
for (int x = 0; x < LONGUEUR; x++) {
int pid = presence_patient(p, x, y);
if (p->dentiste.p.x == x && p->dentiste.p.y == y) {
printf("D ");
} else if (pid) {
printf("P ");
} else {
char c = symboleCase(p->grille[x][y]);
if (c == 'M') printf("# ");
else if (c == 'F') printf("F ");
else if (c == 'T') printf("T ");
else if (c == 'I') printf("I ");
else if (c == 'G') printf("G ");
else if (c == 'R') printf("R ");
else if (c == 'B') printf("B ");
else printf("%c ", c);
}
}
/* Infos patients sur la meme ligne */
if (y >= 4 && y <= 7) {
int idx = y - 4;
Patient* pat = &p->patients[idx];
if (pat->occupe_fauteuil) {
printf(" Patient %d [%s] ", idx+1, noms_pathologies[pat->type_patho]);
afficher_barre_patience(pat);
printf(" | Plateau: ");
if (pat->plateau.nb_pose == 0) {
printf("vide");
} else {
for (int k = 0; k < pat->plateau.nb_pose; k++)
printf("%s ", symbole_instrument(pat->plateau.pose[k]));
}
/* Instruments manquants */
printf(" | Manque: ");
int manque = 0;
for (int k = 0; k < pat->patho.nb_instruments; k++) {
if (!instrument_deja_pose(pat, pat->patho.instruments[k])) {
printf("%s ", noms_instruments[pat->patho.instruments[k]]);
manque++;
}
}
if (!manque) printf("[COMPLET]");
}
}
printf("\n");
}

/* Etat du dentiste */
printf("\n─────────────────────────────────────────────\n");
printf("Dentiste pos (%d,%d) | Mains: %s%s | Gants: %s%s\n",
p->dentiste.p.x, p->dentiste.p.y,
noms_instruments[p->dentiste.mains.i],
p->dentiste.mains.salete ? " (SOUILLE)" : "",
p->dentiste.g.porte_gants ? "OUI" : "NON",
p->dentiste.g.gants_sales ? " (SALES)" : "");

/* Stats */
printf("─────────────────────────────────────────────\n");
printf("Argent: %d€ | Tours: %d | satisfaits : %d | mécontents : %d | furieux : %d\n",
p->argent, p->tours,
p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);

/* Commandes */
printf("─────────────────────────────────────────────\n");
printf("Déplacements: 1=gauche 2=bas 5=haut 3=droite | e=action | s=sauvegarder | x=quitter\n");
printf("─────────────────────────────────────────────\n");
}
