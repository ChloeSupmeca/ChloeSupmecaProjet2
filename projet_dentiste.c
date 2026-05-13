#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define LONGUEUR 8
#define HAUTEUR 9
#define N_FAUTEUILS 4
#define SAVE_FILE "sauvegarde.dat"
#define SCORES_FILE "scores.txt"
#define PATHO_FILE "pathologies.txt"
#define COUTS_FILE "couts.txt"

/* ===================== ENUMERATIONS ===================== */

typedef enum {
AUCUN_INSTR,
PINCE,
ECARTEURS,
SERINGUE,
MIROIR,
SONDE,
FRAISE,
DETARTREUSE,
NB_INSTRUMENTS
} Instrument;

typedef enum {
CARIES,
APHTES,
PARODONTITE,
GINGIVITE,
ABCES,
MALOCCLUSION,
NB_PATHOLOGIES
} Pathologie_Type;

typedef enum {
SOL,
FAUTEUIL,
PLATEAU,
INSTRUMENTS,
RECYCLAGE,
BIOLOGIQUE,
GANTS_CASE,
MUR
} Case;

typedef enum {
SATISFAIT,
MECONTENT,
FURIEUX
} Humeur;

/* ===================== STRUCTURES ===================== */

typedef struct {
Instrument i;
int salete; /* 0=propre, 1=souillee */
} Etat_Instrument;

typedef struct {
int porte_gants;
int gants_sales;
} Gants;

typedef struct {
int x;
int y;
} Position;

typedef struct {
Position p;
Gants g;
Etat_Instrument mains;
} Dentiste;

typedef struct {
int nb_instruments;
Instrument instruments[3];
int prix;
} Pathologie;

/* Plateau d'un fauteuil : liste des instruments poses dessus */
typedef struct {
Instrument pose[3];
int nb_pose;
int sale; /* 1 si plateau souille (apres soins) */
} Plateau_Patient;

typedef struct {
int occupe_fauteuil;
int patience; /* temps restant en "tours" avant impatience */
int patience_max;
int temps_attente; /* tours ecoules depuis arrivee */
int traite;
Position p; /* position du fauteuil */
Pathologie patho;
Pathologie_Type type_patho;
Plateau_Patient plateau;
int plateau_pose; /* 1 si le plateau est a cote */
int gratuit; /* 1 si patient ne paiera pas (hygiène mauvaise a l'arrivee) */
} Patient;

typedef struct {
Case grille[LONGUEUR][HAUTEUR];
Patient patients[N_FAUTEUILS];
Dentiste dentiste;
int argent;
int tours;
int patients_satisfaits;
int patients_mecontents;
int patients_furieux;
int partie_terminee;
int score_final;
time_t debut_partie;
} Partie;

/* ===================== DONNEES STATIQUES ===================== */



int couts[NB_INSTRUMENTS];


const char* noms_instruments[NB_INSTRUMENTS] = {
"AUCUN", "PINCE", "ECARTEURS", "SERINGUE", "MIROIR", "SONDE", "FRAISE", "DETARTREUSE"
};

const char* noms_pathologies[NB_PATHOLOGIES] = {
"Caries", "Aphtes", "Parodontite", "Gingivite", "Abces", "Malocclusion"
};

/* Pathologies avec leurs instruments */
/*Pathologie pathologies_data[NB_PATHOLOGIES] = {
//{1, {DETARTREUSE, AUCUN_INSTR, AUCUN_INSTR}}, /* CARIES */
//{1, {MIROIR, AUCUN_INSTR, AUCUN_INSTR}}, /* APHTES */
//{1, {PINCE, AUCUN_INSTR, AUCUN_INSTR}}, /* PARODONTITE */
//{2, {ECARTEURS, DETARTREUSE, AUCUN_INSTR}}, /* GINGIVITE */
//{2, {SONDE, SERINGUE, AUCUN_INSTR}}, /* ABCES */
//{2, {ECARTEURS, FRAISE, AUCUN_INSTR}} /* MALOCCLUSION */
//};*/

Pathologie pathologies_data[NB_PATHOLOGIES];

/* ===================== UTILITAIRES ===================== */

/* Calcul du prix d'une prestation (somme des couts des instruments necessaires) */
int prix_prestation(Pathologie* patho) {
 
    return patho->prix;

}

/* Noms courts pour affichage */
const char* symbole_instrument(Instrument i) {
switch(i) {
case PINCE: return "PIN";
case ECARTEURS: return "ECA";
case SERINGUE: return "SER";
case MIROIR: return "MIR";
case SONDE: return "SON";
case FRAISE: return "FRA";
case DETARTREUSE:return "DET";
default: return "---";
}
}

/* Retourne 1 si l'instrument est dans la pathologie du patient */
int instrument_utile(Patient* pat, Instrument instr) {
for (int i = 0; i < pat->patho.nb_instruments; i++) {
if (pat->patho.instruments[i] == instr) return 1;
}
return 0;
}

/* Retourne 1 si l'instrument est deja sur le plateau */
int instrument_deja_pose(Patient* pat, Instrument instr) {
for (int i = 0; i < pat->plateau.nb_pose; i++) {
if (pat->plateau.pose[i] == instr) return 1;
}
return 0;
}

/* Retourne 1 si tous les instruments necessaires sont sur le plateau */
int plateau_complet(Patient* pat) {
for (int i = 0; i < pat->patho.nb_instruments; i++) {
if (!instrument_deja_pose(pat, pat->patho.instruments[i])) return 0;
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

/* ===================== INITIALISATION ===================== */

void init_grille(Partie* p) {
int x, y;
for (x = 0; x < LONGUEUR; x++)
for (y = 0; y < HAUTEUR; y++)
p->grille[x][y] = SOL;

/* Fauteuils */
p->grille[7][4] = FAUTEUIL;
p->grille[7][5] = FAUTEUIL;
p->grille[7][6] = FAUTEUIL;
p->grille[7][7] = FAUTEUIL;
/* Plateaux */
p->grille[6][4] = PLATEAU;
p->grille[6][5] = PLATEAU;
p->grille[6][6] = PLATEAU;
p->grille[6][7] = PLATEAU;
/* Instruments */
p->grille[0][2] = INSTRUMENTS;
p->grille[0][3] = INSTRUMENTS;
p->grille[0][4] = INSTRUMENTS;
p->grille[0][5] = INSTRUMENTS;
p->grille[0][6] = INSTRUMENTS;
p->grille[2][0] = INSTRUMENTS;
p->grille[3][0] = INSTRUMENTS;
p->grille[4][0] = INSTRUMENTS;
/* Poubelles */
p->grille[3][8] = RECYCLAGE;
p->grille[4][8] = BIOLOGIQUE;
/* Gants */
p->grille[5][0] = GANTS_CASE;
/* Murs */
p->grille[0][0] = MUR; p->grille[1][0] = MUR;
p->grille[6][0] = MUR; p->grille[7][0] = MUR;
p->grille[0][1] = MUR;
p->grille[0][7] = MUR;
p->grille[0][8] = MUR; p->grille[1][8] = MUR; p->grille[2][8] = MUR;
p->grille[5][8] = MUR; p->grille[6][8] = MUR; p->grille[7][8] = MUR;
p->grille[7][1] = MUR; p->grille[7][2] = MUR; p->grille[7][3] = MUR;
}

void nouvelle_partie(Partie* p) {
memset(p, 0, sizeof(Partie));
init_grille(p);
p->dentiste.p.x = 3;
p->dentiste.p.y = 3;
p->dentiste.mains.i = AUCUN_INSTR;
p->debut_partie = time(NULL);
for (int i = 0; i < N_FAUTEUILS; i++) {
p->patients[i].occupe_fauteuil = 0;
p->patients[i].p.x = 7;
p->patients[i].p.y = 4 + i;
}
}

/* ===================== ARRIVEE DES PATIENTS ===================== */

void faire_arriver_patient(Partie* p) {
/* Cherche un fauteuil libre */
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
pat->patho = pathologies_data[pat->type_patho];
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
/* 1 chance sur 8 par tour qu'un patient arrive si fauteuil libre */
int fauteuil_libre = 0;
for (int i = 0; i < N_FAUTEUILS; i++)
if (!p->patients[i].occupe_fauteuil) { fauteuil_libre = 1; break; }

if (fauteuil_libre && (rand() % 8 == 0)) {
faire_arriver_patient(p);
}
}

/* ===================== PATIENCE ET DEPARTS FORCES ===================== */

void gerer_patience(Partie* p) {
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

void charger_couts() {
    FILE* f = fopen(COUTS_FILE, "r");

    if (!f) {
        printf("Erreur ouverture %s\n", COUTS_FILE);
        exit(1);
    }

    int id, cout;

    while (fscanf(f, "%d %d", &id, &cout) == 2) {
        if (id >= 0 && id < NB_INSTRUMENTS) {
            couts[id] = cout;
        }
    }

    fclose(f);
}

void charger_pathologies() {
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
                pathologies_data[idx].prix = atoi(token);
                break;
            }

            pathologies_data[idx].instruments[nb_instr++] =
                instrument_depuis_nom(token);
        }

        pathologies_data[idx].nb_instruments = nb_instr;

        idx++;
    }

    fclose(f);
}

void action_jeter_gants(Partie* p) {
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
/* ===================== AFFICHAGE ===================== */

void afficher_barre_patience(Patient* pat) {
int pct = (pat->patience * 10) / (pat->patience_max > 0 ? pat->patience_max : 1);
/* Couleurs ANSI */
if (pct > 6) printf("\033[32m"); /* vert */
else if (pct > 3) printf("\033[33m"); /* jaune */
else printf("\033[31m"); /* rouge */

printf("[");
for (int k = 0; k < 10; k++) printf(k < pct ? "#" : ".");
printf("] %2d", pat->patience);
printf("\033[0m");
}

void affichage(Partie* p) {
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
printf("\033[36mD\033[0m ");
} else if (pid) {
printf("\033[35mP\033[0m ");
} else {
char c = symboleCase(p->grille[x][y]);
if (c == 'M') printf("\033[90m#\033[0m ");
else if (c == 'F') printf("\033[33mF\033[0m ");
else if (c == 'T') printf("\033[34mT\033[0m ");
else if (c == 'I') printf("\033[32mI\033[0m ");
else if (c == 'G') printf("\033[32mG\033[0m ");
else if (c == 'R') printf("\033[33mR\033[0m ");
else if (c == 'B') printf("\033[31mB\033[0m ");
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
if (!manque) printf("\033[32m[COMPLET]\033[0m");
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
printf("Argent: \033[32m%d€\033[0m | Tours: %d | ✓ %d ~ %d ✗ %d\n",
p->argent, p->tours,
p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);

/* Commandes */
printf("─────────────────────────────────────────────\n");
printf("Déplacements: 5=gauche 2=bas 1=haut 3=droite | e=action | s=sauvegarder | x=quitter\n");
printf("─────────────────────────────────────────────\n");
}

/* ===================== ACTIONS ===================== */

/* Prendre un instrument */
void action_prendre_instrument(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == INSTRUMENTS) {
if (d->mains.i != AUCUN_INSTR) {
printf("[!] Vous portez déjà un objet !\n");
return;
}
/* Choisir quel instrument prendre */
printf("Quel instrument prendre ?\n");
for (int k = 1; k < NB_INSTRUMENTS; k++) {
printf(" %d: %s (cout: %d€)\n", k, noms_instruments[k], couts[k]);
}
printf(" 0: Annuler\n> ");
int choix;
scanf("%d", &choix);
if (choix <= 0 || choix >= NB_INSTRUMENTS) return;

d->mains.i = (Instrument)choix;
d->mains.salete = !d->g.porte_gants || d->g.gants_sales; /* souille si pas de gants propres */
p->argent -= couts[choix]; /* cout de l'instrument */
printf("[+] Instrument %s pris%s\n", noms_instruments[choix],
d->mains.salete ? " (SOUILLÉ car pas de gants propres)" : "");
return;
}
}
printf("[!] Pas d'instruments a portée.\n");
}

/* Prendre des gants */
void action_prendre_gants(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == GANTS_CASE) {
if (d->g.porte_gants) {
printf("[!] Vous portez déjà des gants !\n");
return;
}
d->g.porte_gants = 1;
d->g.gants_sales = 0;
p->argent -= 1; /* cout des gants */
printf("[+] Gants enfilés !\n");
return;
}
}
printf("[!] Pas de gants à portée.\n");
}

/* Deposer un instrument sur un plateau de patient */
void action_deposer_plateau(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

int idx_patient;
if (p->grille[nx][ny] == PLATEAU && plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];

if (d->mains.i == AUCUN_INSTR) {
printf("[!] Vous ne portez rien.\n");
return;
}
if (d->mains.salete) {
printf("[!] L'instrument est souillé, impossible de le poser sur un plateau stérile !\n");
return;
}
if (!d->g.porte_gants || d->g.gants_sales) {
printf("[!] Vous n'avez pas de gants propres !\n");
return;
}
if (!instrument_utile(pat, d->mains.i)) {
printf("[!] Cet instrument n'est pas nécessaire pour ce patient !\n");
return;
}
if (instrument_deja_pose(pat, d->mains.i)) {
printf("[!] Cet instrument est déjà sur le plateau !\n");
return;
}

pat->plateau.pose[pat->plateau.nb_pose++] = d->mains.i;
d->mains.i = AUCUN_INSTR;
d->mains.salete = 0;
printf("[+] Instrument déposé sur le plateau du patient %d.\n", idx_patient + 1);
return;
}
}
printf("[!] Pas de plateau à portée.\n");
}

/* Soigner un patient */
void action_soigner(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

int idx_patient;
if (p->grille[nx][ny] == PLATEAU && plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];

if (d->mains.i != AUCUN_INSTR) {
printf("[!] Vous portez un objet, impossible de soigner !\n");
return;
}
if (!d->g.porte_gants || d->g.gants_sales) {
/* Patient prend peur, part furieux, plateau souille */
printf("[!!!] Le patient prend peur (gants sales/absents) et part furieux !\n");
pat->plateau.sale = 1;
pat->occupe_fauteuil = 0;
p->patients_furieux++;
d->g.gants_sales = 1;

int tous_pleins = 1;
for (int i = 0; i < N_FAUTEUILS; i++)
if (!p->patients[i].occupe_fauteuil) { tous_pleins = 0; break; }
if (tous_pleins) { p->partie_terminee = 1; p->score_final = p->argent; }
return;
}
if (!plateau_complet(pat)) {
printf("[!] Plateau incomplet, impossible de soigner !\n");
return;
}

/* Soins effectues */
int prix = prix_prestation(&pat->patho);
pat->traite = 1;
pat->plateau.sale = 1;
d->g.gants_sales = 1; /* gants useges apres soins */

Humeur h;
if (pat->patience > pat->patience_max / 2) {
h = SATISFAIT;
if (!pat->gratuit) p->argent += prix;
p->patients_satisfaits++;
printf("[✓] Patient %d soigné avec succès ! Paiement: %d€\n", idx_patient+1, pat->gratuit ? 0 : prix);
} else if (pat->patience > 0) {
h = MECONTENT;
if (!pat->gratuit) p->argent += prix / 2;
p->patients_mecontents++;
printf("[~] Patient %d soigné mais mécontent. Paiement: %d€\n", idx_patient+1, pat->gratuit ? 0 : prix/2);
} else {
h = MECONTENT;
p->patients_mecontents++;
printf("[~] Patient %d mécontent (trop attendu). Paiement: 0€\n", idx_patient+1);
}

(void)h;
pat->occupe_fauteuil = 0; /* patient part mais plateau reste jusqu'au nettoyage */
return;
}
}
printf("[!] Pas de patient à soigner à portée.\n");
}

/* Vider le plateau dans la poubelle biologique */
void action_vider_plateau_biologique(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == BIOLOGIQUE) {
/* Le dentiste doit porter quelque chose de sale (le plateau implicitement) */
/* On cherche un plateau sale parmi les patients traites */
int trouve = 0;
for (int i = 0; i < N_FAUTEUILS; i++) {
Patient* pat = &p->patients[i];
if (!pat->occupe_fauteuil && pat->plateau.sale && pat->plateau.nb_pose > 0) {
/* Verifie que le dentiste est a cote du plateau */
int px = 6, py = 4 + i;
int adj = (abs(d->p.x - px) + abs(d->p.y - py) == 1) ||
(d->p.x == px && d->p.y == py);
/* On simplifie: si a cote de la bio on peut vider n'importe quel plateau qu'on "porte" */
/* Selon l'enonce on prend le plateau et jette son contenu dans la bio */
/* On simplifie: le dentiste doit etre passe par le plateau pour le "prendre" */
/* Ici on verifie juste qu'un plateau sale existe */
(void)adj;
pat->plateau.nb_pose = 0;
pat->plateau.sale = 0;
d->g.porte_gants = 0; /* gants jetes en meme temps */
d->g.gants_sales = 0;
printf("[+] Plateau du patient %d nettoyé, gants jetés.\n", i + 1);
trouve = 1;
break;
}
}
if (!trouve) printf("[!] Aucun plateau sale à vider.\n");
return;
}
}
printf("[!] Pas de poubelle biologique à portée.\n");
}

/* Jeter un instrument dans le recyclage */
void action_jeter_recyclage(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == RECYCLAGE) {
if (d->mains.i == AUCUN_INSTR) {
printf("[!] Vous ne portez rien à recycler.\n");
return;
}
printf("[+] %s jeté dans le bac de recyclage.\n", noms_instruments[d->mains.i]);
d->mains.i = AUCUN_INSTR;
d->mains.salete = 0;
return;
}
}
printf("[!] Pas de bac de recyclage à portée.\n");
}

/* Action principale "e" : detecte ce qui est a cote et agit en consequence */
void action_e(Partie* p) {
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

/* Priorite : gants > recyclage > biologique > plateau */
for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
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

    /* Priorité : jeter les gants si le joueur en porte */
    if (d->g.porte_gants) {
        action_jeter_gants(p);
    } else {
        action_vider_plateau_biologique(p);
    }

    return;
}
if (c == INSTRUMENTS) {
action_prendre_instrument(p);
return;
}
if (c == PLATEAU) {
int idx_patient;
if (plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];
/* Si plateau complet et patient present : soigner */
if (pat->occupe_fauteuil && plateau_complet(pat) &&
d->mains.i == AUCUN_INSTR &&
d->g.porte_gants && !d->g.gants_sales) {
action_soigner(p);
} else if (pat->occupe_fauteuil) {
/* Deposer instrument */
action_deposer_plateau(p);
} else if (!pat->occupe_fauteuil && pat->plateau.sale) {
/* Plateau a vider - on redirige vers bio */
printf("[!] Plateau sale - allez à la poubelle biologique (B) pour le vider.\n");
}
}
return;
}
}
printf("[!] Aucune action possible ici.\n");
}

/* ===================== DEPLACEMENTS ===================== */

void deplacer(Partie* p, int dx, int dy) {
int nx = p->dentiste.p.x + dx;
int ny = p->dentiste.p.y + dy;

if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) return;
if (p->grille[nx][ny] == MUR) return;
if (p->grille[nx][ny] == FAUTEUIL) return; /* ne peut pas marcher sur un fauteuil */

p->dentiste.p.x = nx;
p->dentiste.p.y = ny;
}

/* ===================== SAUVEGARDE ===================== */

void sauvegarder(Partie* p) {
FILE* f = fopen(SAVE_FILE, "wb");
if (!f) { printf("[!] Erreur de sauvegarde.\n"); return; }
fwrite(p, sizeof(Partie), 1, f);
fclose(f);
printf("[+] Partie sauvegardée.\n");
}

int charger_sauvegarde(Partie* p) {
FILE* f = fopen(SAVE_FILE, "rb");
if (!f) return 0;
fread(p, sizeof(Partie), 1, f);
fclose(f);
return 1;
}

int fichier_sauvegarde_existe() {
FILE* f = fopen(SAVE_FILE, "rb");
if (f) { fclose(f); return 1; }
return 0;
}

/* ===================== SCORES ===================== */

void enregistrer_score(Partie* p) {
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

/* ===================== MENU PRINCIPAL ===================== */

void afficher_menu_principal() {
printf("\033[2J\033[H");
printf("╔══════════════════════════════════════╗\n");
printf("║ CABINET DENTAIRE ║\n");
printf("╠══════════════════════════════════════╣\n");
printf("║ 1. Nouvelle partie ║\n");
printf("║ 2. Continuer une partie sauvegardée ║\n");
printf("║ 3. Quitter ║\n");
printf("╚══════════════════════════════════════╝\n");
printf("> ");
}

/* ===================== BOUCLE PRINCIPALE ===================== */

void boucle_jeu(Partie* p) {
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
if (scanf(" %c", &cmd) != 1) continue;

switch(cmd) {
case '5': deplacer(p, -1, 0); break; /* gauche */
case '3': deplacer(p, 1, 0); break; /* droite */
case '1': deplacer(p, 0, -1); break; /* haut */
case '2': deplacer(p, 0, 1); break; /* bas */
case 'e': case 'E': action_e(p); break;
case 's': case 'S': sauvegarder(p); break;
case 'x': case 'X':
printf("Voulez-vous sauvegarder avant de quitter ? (o/n) ");
char rep;
scanf(" %c", &rep);
if (rep == 'o' || rep == 'O') sauvegarder(p);
p->partie_terminee = 1;
p->score_final = p->argent;
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
remove(SAVE_FILE);
}

/* ===================== MAIN ===================== */

int main(void) {
srand((unsigned)time(NULL));
    charger_couts();
charger_pathologies();
Partie p;

int choix = 1;
if (fichier_sauvegarde_existe()) {
afficher_menu_principal();
scanf("%d", &choix);
if (choix == 3) return 0;
if (choix == 2) {
if (!charger_sauvegarde(&p)) {
printf("[!] Echec du chargement, nouvelle partie.\n");
nouvelle_partie(&p);
} else {
printf("[+] Partie chargée !\n");
}
} else {
nouvelle_partie(&p);
}
} else {
nouvelle_partie(&p);
/* Faire arriver un premier patient immédiatement */
faire_arriver_patient(&p);
}

boucle_jeu(&p);
return 0;
}
