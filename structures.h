#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define LONGUEUR 8
#define HAUTEUR 9
#define N_FAUTEUILS 4

typedef enum{
    AUCUN,
    PINCE,
    ECARTEURS,
    SERINGUE,
    MIROIR,
    SONDE,
    FRAISE,
    DETARTREUSE,
    NB_INSTRUMENTS
}Instrument;

typedef enum{
    CARIES,
    APHTES,
    PARODONTITE,
    GINGIVITE,
    ABCES,
    MALOCCLUSION,
    NB_PATHOLOGIES
} Pathologie_Type;

typedef enum {
    SATISFAIT,
    MECONTENT,
    FURIEUX
} Humeur;

typedef enum {
    SOL,
    FAUTEUIL,
    PLATEAU,
    INSTRUMENTS,
    RECYCLAGE,
    BIOLOGIQUE,
    GANTS_PLACE,
    MUR
} Case;

typedef struct{
    Instrument i;
    int salete;
}Etat_Instrument;

typedef struct {
    int porte_gant;
    int salete_gants;
} Gants; 

typedef struct{
    int x;
    int y;
} Position;

typedef struct{
    Position p;
    Gants g;
    Etat_Instrument mains;
}Dentiste;

typedef struct {
    int nb_instruments;
    Instrument instruments[2]; 
} Pathologie; 

typedef struct {
    Instrument pose[2];
    int nb_pose;
    int sale; /* 1 si plateau souille (apres soins) */
} Table_Patient;

typedef struct {
    int occupe_fauteuil;
    int patience;
    int patience_max;
    int temps_attente; //temps ecoulé depuis arrivée 
    int traite;
    Pathologie_Type type_patho;
    Table_Patient table;
    int table_pose;   
    int gratuit;//patient furieux part sans payer
    Position p;
    Pathologie patho;
} Patient;



typedef struct {
    Case grille[LONGUEUR][HAUTEUR];
    Patient patients[N_FAUTEUILS];
    Dentiste dentiste;
    int argent;
    int temps;
    int patients_satisfaits;
    int patients_mecontents;
    int patients_furieux;
    int partie_terminee;
    int score_final;
    time_t debut_partie;
} Partie;
