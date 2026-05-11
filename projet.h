#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define LONGUEUR 20
#define LARGEUR 10

typedef enum{ // outils dentaires
      FRAISE_DENTAIRE=1,  
}Outil;   

typedef enum{  // il s'agit de la présence de gants
      NON = 0, OUI=1,
}Gants; 


typedef struct{ // création du dentiste
      int position[LONGUEUR][LARGEUR];
      Outil ouinon;
      Gants gants;
}Dentiste;

