/* Gestion de l'etat de la partie: initialisation,
  chargement des fichiers de donnees, arrivee/ patience, 
  sauvegarde et enregistrement des scores*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "utils.h"
#include "partie.h"


void init_grille(Partie* p) {
    if (p == NULL) return;

    for (int x = 0; x < LONGUEUR; x++)
        for (int y = 0; y < HAUTEUR; y++)
            p->grille[x][y] = SOL;

    
    p->grille[7][4] = FAUTEUIL;
    p->grille[7][5] = FAUTEUIL;
    p->grille[7][6] = FAUTEUIL;
    p->grille[7][7] = FAUTEUIL;

    
    p->grille[6][4] = PLATEAU;
    p->grille[6][5] = PLATEAU;
    p->grille[6][6] = PLATEAU;
    p->grille[6][7] = PLATEAU;

    
    p->grille[0][2] = INSTRUMENTS;
    p->grille[0][3] = INSTRUMENTS;
    p->grille[0][4] = INSTRUMENTS;
    p->grille[0][5] = INSTRUMENTS;
    p->grille[0][6] = INSTRUMENTS;
    p->grille[2][0] = INSTRUMENTS;
    p->grille[3][0] = INSTRUMENTS;
    p->grille[4][0] = INSTRUMENTS;

    
    p->grille[3][8] = RECYCLAGE;
    p->grille[4][8] = BIOLOGIQUE;

    
    p->grille[5][0] = GANTS_CASE;

    
    p->grille[0][0] = MUR;  p->grille[1][0] = MUR;
    p->grille[6][0] = MUR;  p->grille[7][0] = MUR;
    p->grille[0][1] = MUR;
    p->grille[0][7] = MUR;
    p->grille[0][8] = MUR;  p->grille[1][8] = MUR;  p->grille[2][8] = MUR;
    p->grille[5][8] = MUR;  p->grille[6][8] = MUR;  p->grille[7][8] = MUR;
    p->grille[7][1] = MUR;  p->grille[7][2] = MUR;  p->grille[7][3] = MUR;
}

void nouvelle_partie(Partie* p) {
    if (p == NULL) return;
    memset(p, 0, sizeof(Partie));
    init_grille(p);
    p->dentiste.p.x = 3;
    p->dentiste.p.y = 3;
    p->dentiste.porte_plateau = 0;
    p->dentiste.mains.i = AUCUN_INSTR;
    p->debut_partie = time(NULL);
    for (int i = 0; i < N_FAUTEUILS; i++) {
        p->patients[i].occupe_fauteuil = 0;
        p->patients[i].p.x = 7;
        p->patients[i].p.y = 4 + i;
    }
}



Instrument instrument_depuis_nom(char* nom) {
    if (strcmp(nom, "PINCE")       == 0) return PINCE;
    if (strcmp(nom, "ECARTEURS")   == 0) return ECARTEURS;
    if (strcmp(nom, "SERINGUE")    == 0) return SERINGUE;
    if (strcmp(nom, "MIROIR")      == 0) return MIROIR;
    if (strcmp(nom, "SONDE")       == 0) return SONDE;
    if (strcmp(nom, "FRAISE")      == 0) return FRAISE;
    if (strcmp(nom, "DETARTREUSE") == 0) return DETARTREUSE;
    return AUCUN_INSTR;
}

void charger_couts(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(COUTS_FILE, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture %s\n", COUTS_FILE);
        exit(1);
    }
    int id, cout;
    while (fscanf(f, "%d %d", &id, &cout) == 2) {
        if (id >= 0 && id < NB_INSTRUMENTS)
            p->couts[id] = cout;
    }
    fclose(f);
}

void charger_pathologies(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(PATHO_FILE, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture %s\n", PATHO_FILE);
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
            
            if (token[0] >= '0' && token[0] <= '9') {
                p->pathologies_data[idx].prix = atoi(token);
                break;
            }
            if (nb_instr < 3) {
                p->pathologies_data[idx].instruments[nb_instr++] =
                    instrument_depuis_nom(token);
            }
        }
        p->pathologies_data[idx].nb_instruments = nb_instr;
        idx++;
    }
    fclose(f);
}



void faire_arriver_patient(Partie* p) {
    if (p == NULL) return;
    int idx = -1;
    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (!p->patients[i].occupe_fauteuil) { idx = i; break; }
    }
    if (idx == -1) return;
  
    int plateau_etait_sale = p->patients[idx].plateau.sale || p->patients[idx].plateau.nb_pose > 0;
  
    Patient* pat = &p->patients[idx];
    memset(pat, 0, sizeof(Patient));
    pat->occupe_fauteuil = 1;
    pat->p.x = 7;
    pat->p.y = 4 + idx;
    pat->type_patho   = rand() % NB_PATHOLOGIES;
    pat->patho        = p->pathologies_data[pat->type_patho];
    pat->patience_max = 15 + rand() % 20;
    pat->patience     = pat->patience_max;
    pat->plateau.nb_pose = 0;
    pat->plateau.sale    = 0;
    pat->plateau_pose    = 1;
    pat->gratuit         = 0;

    if (plateau_etait_sale) {
        pat->gratuit = 1;
        printf("[!] Plateau non nettoyé : le patient ne paiera pas !\n");
    }
    /*if (p->patients[idx].plateau.sale || p->patients[idx].plateau.nb_pose > 0) {
        pat->gratuit = 1;
        printf("[!] Le plateau n'est pas propre : le patient ne paiera pas !\n");
    }*/

    printf("\n[!] Patient arrive fauteuil %d ! Pathologie : %s\n",
           idx + 1, noms_pathologies[pat->type_patho]);
    printf("    Instruments necessaires : ");
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        printf("%s ", noms_instruments[pat->patho.instruments[i]]);
    }
    printf("\n");
}

void gerer_arrivees(Partie* p) {
    if (p == NULL) return;
    int fauteuil_libre = 0;
    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (!p->patients[i].occupe_fauteuil) { fauteuil_libre = 1; break; }
    }
    if (fauteuil_libre && (rand() % 8 == 0)) {
        faire_arriver_patient(p);
    }
}

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
            printf("\n[!!!] Patient %d furieux ! Il part sans payer !\n", i + 1);
            pat->plateau.sale    = 1;
            pat->occupe_fauteuil = 0;
            p->patients_furieux++;

            
            if (tous_pleins) {
                printf("\n=== FIN DE PARTIE : Cabinet plein et patient furieux ! ===\n");
                p->partie_terminee = 1;
                p->score_final     = p->argent;
                p->fin_naturelle   = 1;
            }
        }
    }
}



void sauvegarder(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(SAVE_FILE, "wb");
    if (f == NULL) {
        fprintf(stderr, "[!] Erreur ouverture fichier sauvegarde.\n");
        return;
    }
    if (fwrite(p, sizeof(Partie), 1, f) != 1) {
        fprintf(stderr, "[!] Erreur ecriture sauvegarde.\n");
    }
    fclose(f);
    printf("[+] Partie sauvegardee.\n");
}

int charger_sauvegarde(Partie* p) {
    if (p == NULL) return 0;
    FILE* f = fopen(SAVE_FILE, "rb");
    if (f == NULL) return 0;
    if (fread(p, sizeof(Partie), 1, f) != 1) {
        fprintf(stderr, "[!] Erreur lecture sauvegarde.\n");
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

int fichier_sauvegarde_existe(void) {
    FILE* f = fopen(SAVE_FILE, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}



void enregistrer_score(Partie* p) {
    if (p == NULL) return;
    FILE* f = fopen(SCORES_FILE, "a");
    if (!f) return;
    time_t maintenant = time(NULL);
    int duree = (int)(maintenant - p->debut_partie);
    fprintf(f,
        "Score: %d€ | Duree: %ds | Tours: %d | "
        "Satisfaits: %d  Mecontents: %d  Furieux: %d\n",
        p->score_final, duree, p->tours,
        p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);
    fclose(f);
    printf("\n[+] Score enregistre dans %s\n", SCORES_FILE);
}
