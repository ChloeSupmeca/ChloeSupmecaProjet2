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
	int pat;
	printf("___________________________________________________________________\n");
    printf("|                    CABINET DENTAIRE                             |\n");
    printf("|__________________________________________________________________|\n\n");

	printf("D=Dentiste  F=Fauteuil  T=Plateau  I=Instruments  G=Gants\n");
    printf("R=Recyclage B=Biologique  M=Mur  P=Patient\n\n");

    int x, y;
    for (y = 0; y < HAUTEUR; y++) {
    	
    	for (x = 0; x < LONGUEUR; x++) {
        
            if (p->dentiste.p.x == x && p->dentiste.p.y == y) {
                printf("D ");
            }
           
            if (presence_patient(p, x, y)) {
                printf("P ");
            }
            else{
                printf("%c ", symboleCase(p->grille[x][y]));
            }
          
    
    	}
    	printf("\n");
   }

    printf("\n -----------------------\n");
    printf("ARGENT : %d\n", p->argent);
    printf("TEMPS  : %d\n", p->temps_attente);

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
    p->grille[5][0]= GANTS_PLACE;
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
p->temps_attente= 0;
p->patients_satisfaits=0; 
p->patients_mecontents=0; 
	p->dentiste.p.x=3;
	p->dentiste.p.y=3;
	p->dentiste.mains.i=AUCUN;
if (y >= 4 && y <= 7) {
    int idx = y - 4;
    Patient* pat = &p->patients[idx];
     if (pat->occupe_fauteuil) {
        printf("  Patient %d [%s] ", idx+1, noms_pathologies[pat->type_patho]);
afficher_barre_patience(pat);
printf(" | TABLE: ");
            if (pat->table.nb_pose == 0) {
                printf("vide");
            } else {
                    for (int k = 0; k < pat->table.nb_pose; k++)
                        printf("%s ", symbole_instrument(pat->table.pose[k]));
                }
}

printf(" | Manque: ");
    int manque = 0;
        for (int k = 0; k < pat->patho.nb_instruments; k++) {
            if (!instrument_deja_pose(pat, pat->patho.instruments[k])) {
                    printf("%s ", noms_instruments[pat->patho.instruments[k]]);
                     manque++;
                    }
                }
                if (!manque) printf("COMPLET");
            }
        
        printf("\n");
printf("\n─────────────────────────────────────────────\n");
    printf("Dentiste pos (%d,%d) | Mains: %s%s | Gants: %s%s\n",
        p->dentiste.p.x, p->dentiste.p.y,
        noms_instruments[p->dentiste.mains.i],
        p->dentiste.mains.salete ? " (SOUILLE)" : "",
        p->dentiste.g.porte_gants ? "OUI" : "NON",
        p->dentiste.g.gants_sales ? " (SALES)" : "");
printf("─────────────────────────────────────────────\n");
    printf("Argent: %d€ | Tours: %d | satisfaits : %d  mecontents : %d  furieux : %d\n",
        p->argent, p->tours,
        p->patients_satisfaits, p->patients_mecontents, p->patients_furieux);
printf("─────────────────────────────────────────────\n");
    printf("Déplacements: 5=gauche 2=bas 1=haut 3=droite | e=action | x=quitter\n"); //s=sauvegarder a rajouter peut etre
    printf("─────────────────────────────────────────────\n");

    }

int main (){
	Partie p;
	Plateau(&p);
	affichage(&p);
	return 0;
}




