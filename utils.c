/* Implementations des fonctions utilitaires et de l'affichage. 
 Ce module ne modifie jamais l'etat de la partie.
 */

#include <stdio.h>
#include "types.h"
#include "utils.h"



const char* noms_instruments[NB_INSTRUMENTS] = {
    "AUCUN", "PINCE", "ECARTEURS", "SERINGUE",
    "MIROIR", "SONDE", "FRAISE", "DETARTREUSE"
};

const char* noms_pathologies[NB_PATHOLOGIES] = {
    "Caries", "Aphtes", "Parodontite", "Gingivite", "Abces", "Malocclusion"
};



int prix_prestation(Pathologie* patho) {
    return patho->prix;
}

const char* symbole_instrument(Instrument i) {
    switch (i) {
        case PINCE:       return "PIN";
        case ECARTEURS:   return "ECA";
        case SERINGUE:    return "SER";
        case MIROIR:      return "MIR";
        case SONDE:       return "SON";
        case FRAISE:      return "FRA";
        case DETARTREUSE: return "DET";
        default:          return "---";
    }
}

char symboleCase(Case b) {
    char symboles[] = {' ', 'F', 'T', 'I', 'R', 'B', 'G', 'M'};
    return symboles[(int)b];
}

int instrument_utile(Patient* pat, Instrument instr) {
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        if (pat->patho.instruments[i] == instr) return 1;
    }
    return 0;
}

int instrument_deja_pose(Patient* pat, Instrument instr) {
    for (int i = 0; i < pat->plateau.nb_pose; i++) {
        if (pat->plateau.pose[i] == instr) return 1;
    }
    return 0;
}

int plateau_complet(Patient* pat) {
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        if (!instrument_deja_pose(pat, pat->patho.instruments[i])) return 0;
    }
    return 1;
}

int presence_patient(Partie* p, int x, int y) {
    if (p == NULL) return 0;
    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (p->patients[i].occupe_fauteuil &&
            p->patients[i].p.x == x &&
            p->patients[i].p.y == y) {
            return i + 1;
        }
    }
    return 0;
}

int plateau_a_patient(Partie* p, int x, int y, int* idx_patient) {
    if (p == NULL) return 0;
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
    if (p == NULL) return 0;
    if (x == 6 && y >= 4 && y <= 7) {
        *idx_patient = y - 4;
        return 1;
    }
    return 0;
}

/* affiche une barre de patience de progression [####......] 
 proportionnelle a la patience restante, suivie du nombre de tours restants */
void afficher_barre_patience(Patient* pat) {
    int pct = (pat->patience * 10) / (pat->patience_max > 0 ? pat->patience_max : 1);
    printf("[");
    for (int k = 0; k < 10; k++) printf(k < pct ? "#" : ".");
    printf("] %2d", pat->patience);
}

/*Efface l'ecran puis redessine : la grille, 
 les informations des patients sur la ligne de leur fauteuil, 
 l'etat du dentiste, les statistiques globales, le rappel des commandes. */
void affichage(Partie* p) {
    if (p == NULL) return;
    printf("\033[2J\033[H");

    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      CABINET DENTAIRE                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n\n");

    printf("D=Dentiste  F=Fauteuil  T=Plateau  I=Instruments  G=Gants\n");
    printf("R=Recyclage  B=Biologique  M=Mur  P=Patient\n\n");

    /* En-tête colonne */
    printf("  ");
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
                printf("%c ", c == 'M' ? '#' : c);
            }
        }

        /* Infos patients sur la même ligne */
        if (y >= 4 && y <= 7) {
            int idx = y - 4;
            Patient* pat = &p->patients[idx];

            if (pat->occupe_fauteuil) {
                printf("  Patient %d [%s] ", idx + 1, noms_pathologies[pat->type_patho]);
                afficher_barre_patience(pat);

                printf(" | Plateau: ");
                if (pat->plateau.nb_pose == 0) {
                    printf("vide");
                } else {
                    for (int k = 0; k < pat->plateau.nb_pose; k++)
                        printf("%s ", symbole_instrument(pat->plateau.pose[k]));
                }

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

            printf(" | Plateau fauteuil: ");
            if (p->patients[idx].plateau.sale)
                printf("sale");
            else if (p->patients[idx].plateau.nb_pose > 0)
                printf("propre");
            else
                printf("vide");
        }
        printf("\n");
    }

    /* État du dentiste */
    printf("\n─────────────────────────────────────────────\n");
    printf("Dentiste pos (%d,%d) | Mains: %s%s | Gants: %s%s | Plateau: %s\n",
        p->dentiste.p.x, p->dentiste.p.y,
        noms_instruments[p->dentiste.mains.i],
        p->dentiste.mains.salete ? " (SOUILLE)" : "",
        p->dentiste.g.porte_gants ? "OUI" : "NON",
        p->dentiste.g.gants_sales ? " (SALES)" : "",
        p->dentiste.porte_plateau
            ? (p->dentiste.plateau_transporte.sale ? "PORTE (sale)" : "PORTE (propre)")
            : "non");

    printf("─────────────────────────────────────────────\n");
    printf("Argent: %d€ | Tours: %d | satisfaits: %d | mecontents: %d | furieux: %d\n",
        p->argent, p->tours,
        p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);

    printf("─────────────────────────────────────────────\n");
    printf("Deplacements: 1=gauche  2=bas  5=haut  3=droite | e=action | s=sauvegarder | x=quitter\n");
    printf("─────────────────────────────────────────────\n");
}
