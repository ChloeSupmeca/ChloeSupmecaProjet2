typedef enum {
    PLATEAU_PROPRE,
    PLATEAU_SALE,
    PLATEAU_ABSENT
} EtatPlateau;


typedef struct {
    Position p;
    Gants g;
    Etat_Instrument mains;

    int porte_plateau;
    Plateau_Patient plateau_transporte;
    p->dentiste.porte_plateau = 0;

} Dentiste;

void action_prendre_plateau_sale(Partie* p) {
    if (p == NULL) return;

    Dentiste* d = &p->dentiste;

    int dx[] = {-1,1,0,0};
    int dy[] = {0,0,-1,1};

    for(int dir=0; dir<4; dir++) {

        int nx = d->p.x + dx[dir];
        int ny = d->p.y + dy[dir];

        if(nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR)
            continue;

        int idx_patient;

        if(p->grille[nx][ny] == PLATEAU &&
           plateau_a_patient(p,nx,ny,&idx_patient)) {

            Patient* pat = &p->patients[idx_patient];

            if(pat->occupe_fauteuil) {
                printf("[!] Le patient est encore là.\n");
                return;
            }

            if(!pat->plateau.sale) {
                printf("[!] Plateau déjà propre.\n");
                return;
            }

            if(d->porte_plateau) {
                printf("[!] Vous portez déjà un plateau.\n");
                return;
            }

            d->porte_plateau = 1;
            d->plateau_transporte = pat->plateau;

            pat->plateau.nb_pose = 0;
            pat->plateau.sale = 0;

            printf("[+] Plateau sale récupéré.\n");
            return;
        }
    }

    printf("[!] Aucun plateau sale à portée.\n");
}

void action_vider_plateau_biologique(Partie* p) {
    if (p == NULL) return;

    Dentiste* d = &p->dentiste;

    int dx[] = {-1,1,0,0};
    int dy[] = {0,0,-1,1};

    for(int dir=0; dir<4; dir++) {

        int nx = d->p.x + dx[dir];
        int ny = d->p.y + dy[dir];

        if(nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR)
            continue;

        if(p->grille[nx][ny] == BIOLOGIQUE) {

            if(!d->porte_plateau) {
                printf("[!] Vous ne portez aucun plateau.\n");
                return;
            }

            d->porte_plateau = 0;

            d->plateau_transporte.nb_pose = 0;
            d->plateau_transporte.sale = 0;

            printf("[+] Plateau vidé et nettoyé.\n");

            return;
        }
    }

    printf("[!] Pas de poubelle biologique à portée.\n");
}

//affichage 

if (p->dentiste.porte_plateau) {
    printf(" | Plateau sale transporté");
}
else if (!pat->occupe_fauteuil && pat->plateau.sale) {
    action_prendre_plateau_sale(p);
}

//plateau nettoyé
pat->occupe_fauteuil = 0;
pat->plateau.nb_pose = 0;
