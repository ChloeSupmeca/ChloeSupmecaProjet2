/* ===================== INITIALISATION ===================== */

void init_grille(Partie* p) {
    if (p == NULL){
      return;
    }
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
    if (p == NULL) {
      return;
}
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
