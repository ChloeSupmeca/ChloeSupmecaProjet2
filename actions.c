#include"structures_projet_dentiste.c"
/* ===================== ACTIONS ===================== */

/* Prendre un instrument */
void action_prendre_instrument(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == INSTRUMENTS) {
if (d->mains.i != AUCUN_INSTR) {
printf("[!] Vous portez déjà un objet !\n");
return;
}
/* Choisir quel instrument prendre */
printf("Quel instrument prendre ?\n");
for (int k = 1; k < NB_INSTRUMENTS; k++) {
printf(" %d: %s (cout: %d€)\n", k, noms_instruments[k], p->couts[k]);
}
printf(" 0: Annuler\n> ");
int choix=0;
if (scanf("%d", &choix) != 1) {
    fprintf(stderr, "[!] Erreur de saisie.\n");
    return; 
}

if (choix <= 0 || choix >= NB_INSTRUMENTS) return;

d->mains.i = (Instrument)choix;
d->mains.salete = !d->g.porte_gants || d->g.gants_sales; /* souille si pas de gants propres */
p->argent -= p->couts[choix]; /* cout de l'instrument */
printf("[+] Instrument %s pris%s\n", noms_instruments[choix],
d->mains.salete ? " (SOUILLÉ car pas de gants propres)" : "");
return;
}
}
printf("[!] Pas d'instruments a portée.\n");
}

/* Prendre des gants */
void action_prendre_gants(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == GANTS_CASE) {
if (d->g.porte_gants) {
printf("[!] Vous portez déjà des gants !\n");
return;
}
d->g.porte_gants = 1;
d->g.gants_sales = 0;
p->argent -= 1; /* cout des gants */
printf("[+] Gants enfilés !\n");
return;
}
}
printf("[!] Pas de gants à portée.\n");
}

/* Deposer un instrument sur un plateau de patient */
void action_deposer_plateau(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

int idx_patient;
if (p->grille[nx][ny] == PLATEAU && plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];

if (d->mains.i == AUCUN_INSTR) {
printf("[!] Vous ne portez rien.\n");
return;
}
if (d->mains.salete) {
printf("[!] L'instrument est souillé, impossible de le poser sur un plateau stérile !\n");
return;
}
if (!d->g.porte_gants || d->g.gants_sales) {
printf("[!] Vous n'avez pas de gants propres !\n");
return;
}
if (!instrument_utile(pat, d->mains.i)) {
printf("[!] Cet instrument n'est pas nécessaire pour ce patient !\n");
return;
}
if (instrument_deja_pose(pat, d->mains.i)) {
printf("[!] Cet instrument est déjà sur le plateau !\n");
return;
}


if (pat->plateau.nb_pose < 3) { // 3 = taille du tableau pose[]
    pat->plateau.pose[pat->plateau.nb_pose++] = d->mains.i;
} else {
    fprintf(stderr, "[!] Plateau plein, impossible d'ajouter.\n");
}
d->mains.i = AUCUN_INSTR;
d->mains.salete = 0;
printf("[+] Instrument déposé sur le plateau du patient %d.\n", idx_patient + 1);
return;
}
}
printf("[!] Pas de plateau à portée.\n");
}

/* Soigner un patient */
void action_soigner(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

int idx_patient;
if (p->grille[nx][ny] == PLATEAU && plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];

if (d->mains.i != AUCUN_INSTR) {
printf("[!] Vous portez un objet, impossible de soigner !\n");
return;
}
if (!d->g.porte_gants || d->g.gants_sales) {
/* Patient prend peur, part furieux, plateau souille */
printf("[!!!] Le patient prend peur (gants sales/absents) et part furieux !\n");
pat->plateau.sale = 1;
pat->occupe_fauteuil = 0;
p->patients_furieux++;
d->g.gants_sales = 1;

int tous_pleins = 1;
for (int i = 0; i < N_FAUTEUILS; i++)
if (!p->patients[i].occupe_fauteuil) { tous_pleins = 0; break; }
if (tous_pleins) { p->partie_terminee = 1; p->score_final = p->argent; }
return;
}
if (!plateau_complet(pat)) {
printf("[!] Plateau incomplet, impossible de soigner !\n");
return;
}

/* Soins effectues */
int prix = prix_prestation(&pat->patho);
pat->traite = 1;
pat->plateau.sale = 1;
d->g.gants_sales = 1; /* gants useges apres soins */

Humeur h;
if (pat->patience > pat->patience_max / 2) {
h = SATISFAIT;
if (!pat->gratuit) p->argent += prix;
p->patients_satisfaits++;
printf("[✓] Patient %d soigné avec succès ! Paiement: %d€\n", idx_patient+1, pat->gratuit ? 0 : prix);
} else if (pat->patience > 0) {
h = MECONTENT;
if (!pat->gratuit) p->argent += prix / 2;
p->patients_mecontents++;
printf("[~] Patient %d soigné mais mécontent. Paiement: %d€\n", idx_patient+1, pat->gratuit ? 0 : prix/2);
} else {
h = MECONTENT;
p->patients_mecontents++;
printf("[~] Patient %d mécontent (trop attendu). Paiement: 0€\n", idx_patient+1);
}

(void)h;
pat->occupe_fauteuil = 0; /* patient part mais plateau reste jusqu'au nettoyage */
return;
}
}
printf("[!] Pas de patient à soigner à portée.\n");
}

/* Vider le plateau dans la poubelle biologique */
void action_vider_plateau_biologique(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == BIOLOGIQUE) {
/* Le dentiste doit porter quelque chose de sale (le plateau implicitement) */
/* On cherche un plateau sale parmi les patients traites */
int trouve = 0;
for (int i = 0; i < N_FAUTEUILS; i++) {
Patient* pat = &p->patients[i];
if (!pat->occupe_fauteuil && pat->plateau.sale && pat->plateau.nb_pose > 0) {
/* Verifie que le dentiste est a cote du plateau */
int px = 6, py = 4 + i;
int adj = (abs(d->p.x - px) + abs(d->p.y - py) == 1) ||
(d->p.x == px && d->p.y == py);
/* On simplifie: si a cote de la bio on peut vider n'importe quel plateau qu'on "porte" */
/* Selon l'enonce on prend le plateau et jette son contenu dans la bio */
/* On simplifie: le dentiste doit etre passe par le plateau pour le "prendre" */
/* Ici on verifie juste qu'un plateau sale existe */
(void)adj;
pat->plateau.nb_pose = 0;
pat->plateau.sale = 0;
d->g.porte_gants = 0; /* gants jetes en meme temps */
d->g.gants_sales = 0;
printf("[+] Plateau du patient %d nettoyé, gants jetés.\n", i + 1);
trouve = 1;
break;
}
}
if (!trouve) printf("[!] Aucun plateau sale à vider.\n");
return;
}
}
printf("[!] Pas de poubelle biologique à portée.\n");
}

/* Jeter un instrument dans le recyclage */
void action_jeter_recyclage(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

if (p->grille[nx][ny] == RECYCLAGE) {
if (d->mains.i == AUCUN_INSTR) {
printf("[!] Vous ne portez rien à recycler.\n");
return;
}
printf("[+] %s jeté dans le bac de recyclage.\n", noms_instruments[d->mains.i]);
d->mains.i = AUCUN_INSTR;
d->mains.salete = 0;
return;
}
}
printf("[!] Pas de bac de recyclage à portée.\n");
}

/* Action principale "e" : detecte ce qui est a cote et agit en consequence */
void action_e(Partie* p) {
    if (p == NULL) return;
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
Dentiste* d = &p->dentiste;

/* Priorite : gants > recyclage > biologique > plateau */
for (int dir = 0; dir < 4; dir++) {
int nx = d->p.x + dx[dir];
int ny = d->p.y + dy[dir];
if (nx < 0 || nx >= LONGUEUR || ny < 0 || ny >= HAUTEUR) continue;

Case c = p->grille[nx][ny];

if (c == GANTS_CASE) {
action_prendre_gants(p);
return;
}
if (c == RECYCLAGE) {
action_jeter_recyclage(p);
return;
}
if (c == BIOLOGIQUE) {

    /* Priorité : jeter les gants si le joueur en porte */
    if (d->g.porte_gants) {
        action_jeter_gants(p);
    } else {
        action_vider_plateau_biologique(p);
    }

    return;
}
if (c == INSTRUMENTS) {
action_prendre_instrument(p);
return;
}
if (c == PLATEAU) {
int idx_patient;
if (plateau_a_patient(p, nx, ny, &idx_patient)) {
Patient* pat = &p->patients[idx_patient];
/* Si plateau complet et patient present : soigner */
if (pat->occupe_fauteuil && plateau_complet(pat) &&
d->mains.i == AUCUN_INSTR &&
d->g.porte_gants && !d->g.gants_sales) {
action_soigner(p);
} else if (pat->occupe_fauteuil) {
/* Deposer instrument */
action_deposer_plateau(p);
} else if (!pat->occupe_fauteuil && pat->plateau.sale) {
/* Plateau a vider - on redirige vers bio */
printf("[!] Plateau sale - allez à la poubelle biologique (B) pour le vider.\n");
}
}
return;
}
}
printf("[!] Aucune action possible ici.\n");
}
