void action_jeter_gants(Partie* p) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    Dentiste* d = &p->dentiste;

    for (int dir = 0; dir < 4; dir++) {
        int nx = d->p.x + dx[dir];
        int ny = d->p.y + dy[dir];

        if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR)
            continue;

        if (p->grille[nx][ny] == BIOLOGIQUE) {

            if (!d->g.porte_gants) {
                printf("[!] Vous ne portez pas de gants.\n");
                return;
            }

            d->g.porte_gants = 0;
            d->g.gants_sales = 0;

            printf("[+] Gants jetés dans la poubelle biologique.\n");
            return;
        }
    }

    printf("[!] Pas de poubelle biologique à portée.\n");
}
