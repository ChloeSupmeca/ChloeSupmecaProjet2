/* toutes les interactions du dentistes avec son environnement : 
- On parcourt les 4 cases adjacentes (DX/DY) à la recherche d'une case cible.
- On verifie les preconditions : si elles ne sont pas remplies, on affiche un message et on return sans modifier l'etat.
- On ne modifie l'etat que si toutes les conditions sont validees.*/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"
#include "partie.h"
#include "actions.h"


static const int DX[4] = {-1, 1, 0, 0};
static const int DY[4] = { 0, 0,-1, 1};


void action_prendre_gants(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        if (p->grille[nx][ny] == GANTS_CASE) {
            if (d->g.porte_gants) {
                printf(" Vous portez deja des gants !\n");
                return;
            }
            d->g.porte_gants = 1;
            d->g.gants_sales  = 0;
            p->argent -= 1;
            printf("[+] Gants enfiles !\n");
            return;
        }
    }
    printf("[!] Pas de gants a portee.\n");
}

void action_jeter_gants(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        if (p->grille[nx][ny] == BIOLOGIQUE) {
            if (!d->g.porte_gants) {
                printf("[!] Vous ne portez pas de gants.\n");
                return;
            }
            d->g.porte_gants = 0;
            d->g.gants_sales  = 0;
            printf("[+] Gants jetes dans la poubelle biologique.\n");
            return;
        }
    }
    printf("[!] Pas de poubelle biologique a portee.\n");
}



void action_prendre_instrument(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        if (p->grille[nx][ny] == INSTRUMENTS) {
            if (d->mains.i != AUCUN_INSTR) {
                printf("[!] Vous portez deja un objet !\n");
                return;
            }
            printf("Quel instrument prendre ?\n");
            for (int k = 1; k < NB_INSTRUMENTS; k++) {
                printf("  %d: %s (cout: %d€)\n", k, noms_instruments[k], p->couts[k]);
            }
            printf("  0: Annuler\n> ");

            int choix = 0;
            if (scanf("%d", &choix) != 1) {
                fprintf(stderr, "[!] Erreur de saisie.\n");
                return;
            }
            if (choix <= 0 || choix >= NB_INSTRUMENTS) return;

            d->mains.i      = (Instrument)choix;
            d->mains.salete = (!d->g.porte_gants || d->g.gants_sales) ? 1 : 0;
            p->argent      -= p->couts[choix];
            printf("[+] Instrument %s pris%s\n",
                noms_instruments[choix],
                d->mains.salete ? " (SOUILLE : pas de gants propres)" : "");
            return;
        }
    }
    printf("[!] Pas d'instruments a portee.\n");
}

void action_jeter_recyclage(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        if (p->grille[nx][ny] == RECYCLAGE) {
            if (d->mains.i == AUCUN_INSTR) {
                printf("[!] Vous ne portez rien a recycler.\n");
                return;
            }
            printf("[+] %s jete dans le bac de recyclage.\n", noms_instruments[d->mains.i]);
            d->mains.i      = AUCUN_INSTR;
            d->mains.salete = 0;
            return;
        }
    }
    printf("[!] Pas de bac de recyclage à portee.\n");
}



void action_deposer_plateau(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        int idx_patient;
        if (p->grille[nx][ny] == PLATEAU &&
            plateau_a_patient(p, nx, ny, &idx_patient)) {

            Patient* pat = &p->patients[idx_patient];

            if (d->mains.i == AUCUN_INSTR) {
                printf("[!] Vous ne portez rien.\n");
                return;
            }
            if (d->mains.salete) {
                printf("[!] Instrument souille, impossible sur un plateau sterile !\n");
                return;
            }
            if (!d->g.porte_gants || d->g.gants_sales) {
                printf("[!] Vous n'avez pas de gants propres !\n");
                return;
            }
            if (!instrument_utile(pat, d->mains.i)) {
                printf("[!] Cet instrument n'est pas necessaire pour ce patient !\n");
                return;
            }
            if (instrument_deja_pose(pat, d->mains.i)) {
                printf("[!] Cet instrument est deja sur le plateau !\n");
                return;
            }
            if (pat->plateau.nb_pose >= 3) {
                fprintf(stderr, "[!] Plateau plein, impossible d'ajouter.\n");
                return;
            }

            pat->plateau.pose[pat->plateau.nb_pose++] = d->mains.i;
            d->mains.i      = AUCUN_INSTR;
            d->mains.salete = 0;
            printf("[+] Instrument depose sur le plateau du patient %d.\n", idx_patient + 1);
            return;
        }
    }
    printf("[!] Pas de plateau à portee.\n");
}

void action_vider_plateau_biologique(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        if (p->grille[nx][ny] == BIOLOGIQUE) {
            if (!d->porte_plateau) {
                printf("[!] Vous ne portez pas de plateau.\n");
                return;
            }
            d->plateau_transporte.nb_pose = 0;
            d->plateau_transporte.sale    = 0;
            d->g.porte_gants              = 0; 
            d->g.gants_sales              = 0;
            printf("[+] Plateau vide dans la poubelle biologique. Gants jetés.\n");
            return;
        }
    }
    printf("[!] Pas de poubelle biologique à portée.\n");
}



void action_soigner(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        int idx_patient;
        if (p->grille[nx][ny] == PLATEAU &&
            plateau_a_patient(p, nx, ny, &idx_patient)) {

            Patient* pat = &p->patients[idx_patient];

            if (d->mains.i != AUCUN_INSTR) {
                printf("[!] Vous portez un objet, impossible de soigner !\n");
                return;
            }
            if (!d->g.porte_gants || d->g.gants_sales) {
                printf("[!!!] Le patient prend peur (gants absents/sales) et part furieux !\n");
                pat->plateau.sale    = 1;
                pat->occupe_fauteuil = 0;
                p->patients_furieux++;
                d->g.gants_sales = 1;

                int tous_pleins = 1;
                for (int i = 0; i < N_FAUTEUILS; i++)
                    if (!p->patients[i].occupe_fauteuil) { tous_pleins = 0; break; }
                if (tous_pleins) {
                    p->partie_terminee = 1;
                    p->score_final     = p->argent;
                    p->fin_naturelle   = 1;
                }
                return;
            }
            if (!plateau_complet(pat)) {
                printf("[!] Plateau incomplet, impossible de soigner !\n");
                return;
            }

            
            int prix = prix_prestation(&pat->patho);
            pat->traite       = 1;
            pat->plateau.sale = 1;
            d->g.gants_sales  = 1;

            if (pat->patience > pat->patience_max / 2) {
                if (!pat->gratuit) p->argent += prix;
                p->patients_satisfaits++;
                printf(" Patient %d soigne ! Paiement : %d€\n",
                    idx_patient + 1, pat->gratuit ? 0 : prix);
            } else if (pat->patience > 0) {
                if (!pat->gratuit) p->argent += prix / 2;
                p->patients_mecontents++;
                printf(" Patient %d mecontent. Paiement : %d€\n",
                    idx_patient + 1, pat->gratuit ? 0 : prix / 2);
            } else {
                p->patients_mecontents++;
                printf(" Patient %d trop attendu. Paiement : 0€\n", idx_patient + 1);
            }

            pat->occupe_fauteuil = 0;
            return;
        }
    }
    printf("[!] Pas de patient a soigner à portee.\n");
}


void action_e(Partie* p) {
    if (p == NULL) return;
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + DX[dir];
        int ny = d->p.y + DY[dir];
        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

        Case c = p->grille[nx][ny];

        if (c == GANTS_CASE) {
            action_prendre_gants(p);
            return;
        }
        if (c == RECYCLAGE) {
            action_jeter_recyclage(p);
            return;
        }
        if (c == BIOLOGIQUE) {
            if (d->porte_plateau) {
                action_vider_plateau_biologique(p);
            } else if (d->g.porte_gants) {
                action_jeter_gants(p);
            } else {
                printf("[!] Rien a jeter dans la poubelle biologique.\n");
            }
            return;
        }
        if (c == INSTRUMENTS) {
            action_prendre_instrument(p);
            return;
        }
        if (c == PLATEAU) {
            int idx_patient;

            
            if (d->porte_plateau && !d->plateau_transporte.sale) {
                int idx;
                if (plateau_a_patient_ou_libre(p, nx, ny, &idx)) {
                    p->patients[idx].plateau = d->plateau_transporte;
                    d->porte_plateau = 0;
                    printf("[+] Plateau remis en place.\n");
                    return;
                }
            }

            
            if (plateau_a_patient(p, nx, ny, &idx_patient)) {
                Patient* pat = &p->patients[idx_patient];
                if (pat->occupe_fauteuil && plateau_complet(pat) &&
                    d->mains.i == AUCUN_INSTR ) { 
                    action_soigner(p);
                    return;
                } else if (pat->occupe_fauteuil) {
                    action_deposer_plateau(p);
                    return;
                }
            }

            
            int idx;
            if (plateau_a_patient_ou_libre(p, nx, ny, &idx)) {
                Patient* pat = &p->patients[idx];
                if (pat->plateau.sale || pat->plateau.nb_pose > 0) {
                    if (d->porte_plateau) {
                        printf("[!] Vous portez deja un plateau.\n");
                    } else {
                        d->porte_plateau      = 1;
                        d->plateau_transporte = pat->plateau;
                        pat->plateau.nb_pose  = 0;
                        pat->plateau.sale     = 0;
                        printf("[+] Plateau recupere (%s).\n",
                            d->plateau_transporte.sale ? "sale" : "propre");
                    }
                } else {
                    printf("[!] Aucun plateau a prendre ici.\n");
                }
            }
            return;
        }
    }
    printf("[!] Aucune action possible ici.\n");
}



void deplacer(Partie* p, int dx, int dy) {
    if (p == NULL) return;
    int nx = p->dentiste.p.x + dx;
    int ny = p->dentiste.p.y + dy;

    if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) return;

    Case c = p->grille[nx][ny];
    if (c == MUR        || c == FAUTEUIL   || c == PLATEAU  ||
        c == INSTRUMENTS|| c == RECYCLAGE  || c == BIOLOGIQUE ||
        c == GANTS_CASE) {
        return;
    }
    p->dentiste.p.x = nx;
    p->dentiste.p.y = ny;
}
