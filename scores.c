#include"structures_projet_dentiste.h"
/* ===================== SCORES ===================== */

void enregistrer_score(Partie* p) {
    if (p == NULL) return;
FILE* f = fopen(SCORES_FILE, "a");
if (!f) return;
time_t maintenant = time(NULL);
int duree = (int)(maintenant - p->debut_partie);
fprintf(f, "Score: %d€ | Duree: %ds | Tours: %d | Satisfaits:%d Mecontents:%d Furieux:%d\n",
p->score_final, duree, p->tours,
p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);
fclose(f);
printf("\n[+] Score enregistré dans %s\n", SCORES_FILE);
}
