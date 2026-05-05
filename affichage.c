#include <structures.h>
char symboleCase (Case b){
    char symboles[] = {'_', 'F', 'I', 'P', ' '};
    return symboles[b];

}
int presence_patient(Partie* p,int x, int y){
    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (p->patients[i].occupe_fauteuil && p->patients[i].p.x == x && p->patients[i].p.y == y) {
            return 1;
        }
    }
    return 0;
}

void affichage(Partie* p){

    int x, y;
    for (x = 0; x < LONGUEUR; x++) {
        for (y = 0; y < HAUTEUR; y++) {
            if (p->dentiste.p.x == x && p->dentiste.p.y == y) {
                printf("D");
            }
            else if (presence_patient(p, x, y)) {
                printf("P");
            }
            else {
                printf("%c", symboleCase(p->grille[x][y]));
            }
        }
    printf("\n ---------------------------------------------\n");
    }
    printf("ARGENT : %d\n", p->argent);
    printf("TEMPS  : %d\n", p->temps);

    printf("Patients satisfaits   : %d\n", p->patients_satisfaits);
    printf("patients mécontents : %d\n", p->patients_mecontents);

    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (p->patients[i].occupe_fauteuil) {
            printf("Patient %d -> patience : %d\n",i +1,p->patients[i].patience);
        }
    }
}

void Plateau(Partie *p) {
    int x, y;

    for (x = 0; x < LONGUEUR; x++) {
        for (y = 0; y < HAUTEUR; y++) {
            p->grille[y][x] = SOL;
        }
    }

    p->grille[3][9] = FAUTEUIL;
    p->grille[10][5] = FAUTEUIL;
    p->grille[10][7] = FAUTEUIL;
    p->grille[10][9] = FAUTEUIL;
    p->grille[3][5] = INSTRUMENTS;
    p->grille[4][5] = INSTRUMENTS;
    p->grille[5][10] = POUBELLE;
    p->grille[7][10] = POUBELLE;
    //mettre les murs et les vides

p->argent = 0;
p->temps = 0;
p->patients_satisfaits; //expression vide ?
p->patients_mecontents; //expression vide ?
}
