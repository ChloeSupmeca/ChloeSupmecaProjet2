#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define LONGUEUR 12
#define HAUTEUR 11
#define N_FAUTEUILS 4

typedef enum{
    AUCUN,
    PINCE,
    ECARTEURS 
}Instrument;

typedef struct{
    Instrument i;
    int salete;
}Etat_Instrument;

typedef struct {
    int gant;
    int salete_gants;
} Gants; 

typedef struct{
    int x;
    int y;
} Position;

typedef struct{
    Position p;
    Gants g;
    Etat_Instrument hand;
}Dentiste;

typedef struct {
    int nb_instruments;
    Instrument instruments[2]; 
} Pathologie; 


typedef struct {
    int occupe_fauteuil;
    int patience;
    /*Pathologie Etat_Instrument[2]; (pathologies a rajouter dans un fichier à part)*/
    int treated;
    Position p;
    Pathologie patho;
} Patient;

typedef enum {
    SOL,
    FAUTEUIL,
    INSTRUMENTS,
    POUBELLE,
    VIDE
} Case;

typedef struct {
    Case grille[LONGUEUR][HAUTEUR];
    Patient patients[N_FAUTEUILS];
    Dentiste dentiste;
    int argent;
    float temps;
    int patients_satisfaits;
    int patients_mecontents;


} Partie;
