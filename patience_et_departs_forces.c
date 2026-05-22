#include"structures_projet_dentiste.h"

/* ===================== PATIENCE ET DEPARTS FORCES ===================== */

void gerer_patience(Partie* p) {
    if (p == NULL) return;
int tous_pleins = 1;
for (int i = 0; i < N_FAUTEUILS; i++) {
if (!p->patients[i].occupe_fauteuil) { tous_pleins = 0; break; }
}

for (int i = 0; i < N_FAUTEUILS; i++) {
Patient* pat = &p->patients[i];
if (!pat->occupe_fauteuil || pat->traite) continue;

pat->temps_attente++;
pat->patience--;

if (pat->patience <= 0) {
/* Patient furieux -> part sans payer */
printf("\n[!!!] Patient %d furieux ! Il part sans payer !\n", i + 1);
/* Plateau souille comme apres soins */
pat->plateau.sale = 1;
pat->occupe_fauteuil = 0;
p->patients_furieux++;

/* Condition de fin : tous pleins + un furieux */
if (tous_pleins) {
printf("\n=== FIN DE PARTIE : Cabinet plein et patient furieux ! ===\n");
p->partie_terminee = 1;
p->score_final = p->argent;
p->fin_naturelle = 1;
}
}
}
}

Instrument instrument_depuis_nom(char* nom) {
    if (strcmp(nom, "PINCE") == 0) return PINCE;
    if (strcmp(nom, "ECARTEURS") == 0) return ECARTEURS;
    if (strcmp(nom, "SERINGUE") == 0) return SERINGUE;
    if (strcmp(nom, "MIROIR") == 0) return MIROIR;
    if (strcmp(nom, "SONDE") == 0) return SONDE;
    if (strcmp(nom, "FRAISE") == 0) return FRAISE;
    if (strcmp(nom, "DETARTREUSE") == 0) return DETARTREUSE;

    return AUCUN_INSTR;
}

void charger_couts(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(COUTS_FILE, "r");

    if (!f) {
        printf("Erreur ouverture %s\n", COUTS_FILE);
        exit(1);
    }

    int id, cout;

    while (fscanf(f, "%d %d", &id, &cout) == 2) {
        if (id >= 0 && id < NB_INSTRUMENTS) {
            p->couts[id] = cout;
        }
    }

    fclose(f);
}

void charger_pathologies(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(PATHO_FILE, "r");

    if (!f) {
        printf("Erreur ouverture %s\n", PATHO_FILE);
        exit(1);
    }

    char ligne[256];

    int idx = 0;

    while (fgets(ligne, sizeof(ligne), f) && idx < NB_PATHOLOGIES) {

        char* token = strtok(ligne, " \n");

        if (!token) continue;

        int nb_instr = 0;

        while (1) {

            token = strtok(NULL, " \n");

            if (!token) break;

            /* Si c'est un nombre => prix */
            if (token[0] >= '0' && token[0] <= '9') {
                p->pathologies_data[idx].prix = atoi(token);
                break;
            }

            if(nb_instr < 3){p->pathologies_data[idx].instruments[nb_instr++] =
                instrument_depuis_nom(token);
            }
                
        }

        p->pathologies_data[idx].nb_instruments = nb_instr;

        idx++;
    }

    fclose(f);
}

void action_jeter_gants(Partie* p) {
    if (p == NULL) return;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {

        int nx = d->p.x + dx[dir];
        int ny = d->p.y + dy[dir];

        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR)
            continue;

        if (p->grille[nx][ny] == BIOLOGIQUE) {

            if (!d->g.porte_gants) {
                printf("[!] Vous ne portez pas de gants.\n");
                return;
            }

            d->g.porte_gants = 0;
            d->g.gants_sales = 0;

            printf("[+] Gants jetés dans la poubelle biologique.\n");
            return;
        }
    }

    printf("[!] Pas de poubelle biologique à portée.\n");
}
