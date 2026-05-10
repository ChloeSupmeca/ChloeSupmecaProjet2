#include "structures.h"
char symboleCase (Case b){
    char symboles[] = {' ', 'F','T', 'I', 'R','B','G','M'};
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
    for (y = 0; y < HAUTEUR; y++) {
    	
    	for (x = 0; x < LONGUEUR; x++) {
        
            if (p->dentiste.p.x == x && p->dentiste.p.y == y) {
                printf("D ");
            }
           
            else if (presence_patient(p, x, y)) {
                printf("P ");
            }
            else {
                printf("%c ", symboleCase(p->grille[x][y]));
            }
          
    
    	}
    	printf("\n");
   }

    printf("\n -----------------------\n");
    printf("ARGENT : %d\n", p->argent);
    printf("TEMPS  : %d\n", p->temps);

    printf("Patients satisfaits   : %d\n", p->patients_satisfaits);
    printf("patients mécontents : %d\n", p->patients_mecontents);
	 printf("patients furieux : %d\n", p->patients_furieux);


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
            p->grille[x][y] = SOL;
            
          
        }
    }

    p->grille[7][4] = FAUTEUIL;
    p->grille[7][5] = FAUTEUIL;
    p->grille[7][6] = FAUTEUIL;
    p->grille[7][7] = FAUTEUIL;
	p->grille[6][4] = PLATEAU;
	p->grille[6][5] = PLATEAU;
	p->grille[6][6] = PLATEAU;
	p->grille[6][7] = PLATEAU;
    p->grille[0][2]= INSTRUMENTS;
    p->grille[0][3]= INSTRUMENTS;
    p->grille[0][4]= INSTRUMENTS;
    p->grille[0][5]= INSTRUMENTS;
    p->grille[0][6]= INSTRUMENTS;
    p->grille[3][8] = RECYCLAGE;
    p->grille[4][8] = BIOLOGIQUE;
    p->grille[0][0]= MUR;
    p->grille[1][0]= MUR;
    p->grille[5][0]= GANTS;
    p->grille[6][0]= MUR;
    p->grille[7][0]= MUR;
    p->grille[0][1]= MUR;
    p->grille[0][7]= MUR;
    p->grille[0][8]= MUR;
    p->grille[1][8]= MUR;
    p->grille[2][8]= MUR;
    p->grille[5][8]= MUR;
    p->grille[6][8]= MUR;
    p->grille[7][8]= MUR;
    p->grille[7][0]= MUR;
    p->grille[7][1]= MUR;
    p->grille[7][2]= MUR;
	p->grille[4][0]= INSTRUMENTS; 
    p->grille[2][0]= INSTRUMENTS;
    p->grille[7][3]= MUR;
    p->grille[3][0]= INSTRUMENTS;
    

p->argent = 0;
p->temps = 0;
p->patients_satisfaits=0; 
p->patients_mecontents=0; 
	p->dentiste.p.x=3;
	p->dentiste.p.y=3;
	p->dentiste.mains.i=AUCUN;

	for(int i=0; i<N_FAUTEUILS; i++){
		p->patients[i].occupe_fauteuil=0;
	}
}



int main (){
	Partie p;
	Plateau(&p);
	affichage(&p);
	return 0;
}




