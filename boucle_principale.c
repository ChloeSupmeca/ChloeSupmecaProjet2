#include"structures_projet_dentiste.h"
/* ===================== BOUCLE PRINCIPALE ===================== */

void boucle_jeu(Partie* p) {
    if (p == NULL) return;
char cmd;
while (!p->partie_terminee) {
/* Arrivee aleatoire de patients */
gerer_arrivees(p);

/* Affichage */
affichage(p);

/* Saisie commande */
printf("\nAction > ");
fflush(stdout);
cmd = 0;
if (scanf(" %c", &cmd) != 1){
    continue;
}
switch(cmd) {
case '1': deplacer(p, -1, 0); break; /* gauche */
case '3': deplacer(p, 1, 0); break; /* droite */
case '5': deplacer(p, 0, -1); break; /* haut */
case '2': deplacer(p, 0, 1); break; /* bas */
case 'e': case 'E': action_e(p); break;
case 's': case 'S': sauvegarder(p); break;
case 'x': case 'X':
printf("Voulez-vous sauvegarder avant de quitter ? (o/n) ");
char rep;
if (scanf(" %c", &rep) != 1) {
    fprintf(stderr, "[!] Saisie invalide.\n");
    break;
}
if (rep == 'o' || rep == 'O') sauvegarder(p);
p->partie_terminee = 1;
p->score_final = p->argent;
p->fin_naturelle = 0;
break;
default:
printf("[!] Commande inconnue.\n");
break;
}

p->tours++;
/* Diminuer patience toutes les 3 actions */
if (p->tours % 3 == 0) gerer_patience(p);
}

/* Fin de partie */
printf("\n═══════════════════════════════════\n");
printf(" FIN DE PARTIE\n");
printf("═══════════════════════════════════\n");
printf("Score final : %d€\n", p->score_final);
printf("Tours joués : %d\n", p->tours);
printf("Satisfaits : %d\n", p->patients_satisfaits);
printf("Mécontents : %d\n", p->patients_mecontents);
printf("Furieux : %d\n", p->patients_furieux);
enregistrer_score(p);

/* Supprimer sauvegarde si partie terminee normalement */
if (p->fin_naturelle) {
    remove(SAVE_FILE);
}
}

