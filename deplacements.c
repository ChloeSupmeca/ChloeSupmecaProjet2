#include"structures_projet_dentiste.h"

/* ===================== DEPLACEMENTS ===================== */

void deplacer(Partie* p, int dx, int dy) {
    if (p == NULL) return;
int nx = p->dentiste.p.x + dx;
int ny = p->dentiste.p.y + dy;

if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) return;
if (p->grille[nx][ny] == MUR) return;
if (p->grille[nx][ny] == FAUTEUIL) return; /* ne peut pas marcher sur un fauteuil */
if (p->grille[nx][ny] == PLATEAU) return;
if (p->grille[nx][ny] == INSTRUMENTS) return;
if (p->grille[nx][ny] == RECYCLAGE) return;
if (p->grille[nx][ny] == BIOLOGIQUE) return;
if (p->grille[nx][ny] == GANTS_CASE) return;
p->dentiste.p.x = nx;
p->dentiste.p.y = ny;
}
