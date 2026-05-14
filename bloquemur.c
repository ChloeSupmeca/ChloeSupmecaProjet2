int case_bloquante(Case c) {
   return c == MUR || c == FAUTEUIL || c == PATIENT || c == PORTE_FERMEE;
}

void deplacer(Partie* p, int dx, int dy) {

    int nx = p->dentiste.p.x + dx;
    int ny = p->dentiste.p.y + dy;

    /* Hors limites */
    if (nx < 0 || nx >= LONGUEUR ||ny < 0 || ny >= HAUTEUR){
        return;
     }
    /* Collision */
    if (case_bloquante(p->grille[nx][ny])){
        return;
    }
    p->dentiste.p.x = nx;
    p->dentiste.p.y = ny;
}

