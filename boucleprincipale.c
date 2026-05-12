void boucle_jeu(Partie* p) {
char comm;
while (!p->partie_terminee) {

gerer_arrivees(p);

affichage(p);

printf("\nAction > ");
fflush(stdout);
comm = 0;
if (scanf(" %c", &cmd) != 1) continue;

switch(cmd) {
case '5':
    deplacer(p, -1, 0);
    break; // gauche 
case '3':
    deplacer(p, 1, 0);
    break; // droite 
case '1':
    deplacer(p, 0, -1); 
    break; // haut 
case '2':
    deplacer(p, 0, 1); 
    break; //bas 
case 'e':
    case 'E': 
    action_e(p); 
        break;
/*//case 's': 
        case 'S': 
   // sauvegarder(p); 
            break;*/
case 'x':
    case 'X':
/*printf("Voulez-vous sauvegarder avant de quitter ? (o/n) ");
char rep;
scanf(" %c", &rep);
if (rep == 'o' || rep == 'O') 
sauvegarder(p);*/
p->partie_terminee = 1;
p->score_final = p->argent;
break;
default:
printf("[!] Commande inconnue.\n");
break;
}

p->tours++;
if (p->tours % 3 == 0){
    gerer_patience(p); //on diminue la patience tous kes 3 tours
}

printf("\n----------------------------------\n");
printf(" FIN DE PARTIE\n");
printf("------------------------------------\n");
printf("Score final : %d€\n", p->score_final);
printf("Temps : %d\n", p->tours);
printf("Satisfaits : %d\n", p->patients_satisfaits);
printf("Mécontents : %d\n", p->patients_mecontents);
printf("Furieux : %d\n", p->patients_furieux);
enregistrer_score(p);
//remove(SAVE_FILE);
}

int prix_prestation(Pathologie* patho) {
    int total = 0;
    for (int i = 0; i < patho->nb_instruments; i++) {
        total += couts[patho->instruments[i]];
    }
    return total;
}

int instrument_utile(Patient* pat, Instrument instr) {
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        if (pat->patho.instruments[i] == instr){
            return 1;
        }
    }
    return 0;
}

int instrument_deja_pose(Patient* pat, Instrument instr) {
    for (int i = 0; i < pat->plateau.nb_pose; i++) {
        if (pat->plateau.pose[i] == instr){
            return 1;
    }
    return 0;
    }
}

int plateau_complet(Patient* pat) {
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        if (!instrument_deja_pose(pat, pat->patho.instruments[i])) {
            return 0;
    }
    return 1;
}

int plateau_a_patient(Partie* p, int x, int y, int* idx_patient) {
    if (x == 6 && y >= 4 && y <= 7) {
        for (int i = 0; i < N_FAUTEUILS; i++) {
            if (p->patients[i].occupe_fauteuil &&
                p->patients[i].p.y == y) {
                *idx_patient = i;
                return 1;
            }
        }
    }
    return 0;
}
void nouvelle_partie(Partie* p) {
    memset(p, 0, sizeof(Partie));
    init_grille(p);
    p->dentiste.p.x = 3;
    p->dentiste.p.y = 3;
    p->dentiste.mains.i = AUCUN;
    p->debut_partie = time(NULL);
    for (int i = 0; i < N_FAUTEUILS; i++) {
        p->patients[i].occupe_fauteuil = 0;
        p->patients[i].p.x = 7;
        p->patients[i].p.y = 4 + i;
    }
}

void faire_arriver_patient(Partie* p) {
    int idx = -1;
    for (int i = 0; i < N_FAUTEUILS; i++) {
        if (!p->patients[i].occupe_fauteuil) { idx = i; break; }
    }
    if (idx == -1) return;

    Patient* pat = &p->patients[idx];
    memset(pat, 0, sizeof(Patient));
    pat->occupe_fauteuil = 1;
    pat->p.x = 7;
    pat->p.y = 4 + idx;
    pat->type_patho = rand() % NB_PATHOLOGIES;
    pat->patho = pathologies_data[pat->type_patho];
    pat->patience_max = 15 + rand() % 20; /* entre 15 et 34 tours */
    pat->patience = pat->patience_max;
    pat->plateau.nb_pose = 0;
    pat->plateau.sale = 0;
    pat->plateau_pose = 1; /* le plateau est toujours a cote du fauteuil */
    pat->gratuit = 0;

    /* Si un plateau sale est encore present (ancien patient), le nouveau est mecontent -> gratuit */
    /* On verifie si le plateau en x=6,y=4+idx est "sale" (ancien contenu) */
    /* Ici on gere ca via le flag gratuit */

    printf("\n[!] Un patient arrive en fauteuil %d ! Pathologie : %s\n",
           idx + 1, noms_pathologies[pat->type_patho]);
    printf("    Instruments necessaires : ");
    for (int i = 0; i < pat->patho.nb_instruments; i++) {
        printf("%s ", noms_instruments[pat->patho.instruments[i]]);
    }
    printf("\n");
}

