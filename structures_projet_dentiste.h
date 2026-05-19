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

typedef enum {
    PLATEAU_PROPRE,
    PLATEAU_SALE,
    PLATEAU_ABSENT
} EtatPlateau;


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
    Position p;
    Gants g;
    Etat_Instrument mains;

    int porte_plateau;
    Plateau_Patient plateau_transporte;
    p->dentiste.porte_plateau = 0;

} Dentiste;


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
  int couts[NB_INSTRUMENTS];
  Pathologie pathologies_data[NB_PATHOLOGIES];
  int argent;
  int tours;
  int patients_satisfaits;
  int patients_mecontents;
  int patients_furieux;
  int partie_terminee;
  int score_final;
  time_t debut_partie;
} Partie;
