/* Declarations centrales du projet : constantes , enumerations, et structures. 
 Fichier inclus par tous les autres modules */

#ifndef TYPES_H
#define TYPES_H

#include <time.h>

#define LONGUEUR    8
#define HAUTEUR     9
#define N_FAUTEUILS 4
#define SAVE_FILE   "sauvegarde.dat"
#define SCORES_FILE "scores.txt"
#define PATHO_FILE  "pathologies.txt"
#define COUTS_FILE  "couts.txt"



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



typedef struct {
    Instrument i;
    int salete; 
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
    int nb_instruments;
    Instrument instruments[3];
    int prix;
} Pathologie;

typedef struct {
    Instrument pose[3];
    int nb_pose;
    int sale; 
} Plateau_Patient;

typedef struct {
    Position p;
    Gants g;
    Etat_Instrument mains;
    int porte_plateau;
    Plateau_Patient plateau_transporte;
} Dentiste;

typedef struct {
    int occupe_fauteuil;
    int patience;
    int patience_max;
    int temps_attente;
    int traite;
    Position p;
    Pathologie patho;
    Pathologie_Type type_patho;
    Plateau_Patient plateau;
    int plateau_pose;
    int gratuit;
} Patient;

typedef struct {
    Case grille[LONGUEUR][HAUTEUR];
    Patient patients[N_FAUTEUILS];
    Dentiste dentiste;
    int couts[NB_INSTRUMENTS];
    Pathologie pathologies_data[NB_PATHOLOGIES];
    int argent;
    int tours;
    int patients_satisfaits;
    int patients_mecontents;
    int patients_furieux;
    int partie_terminee;
    int score_final;
    int fin_naturelle;
    time_t debut_partie;
} Partie;



extern const char* noms_instruments[NB_INSTRUMENTS];
extern const char* noms_pathologies[NB_PATHOLOGIES];

#endif 
