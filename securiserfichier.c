int choix;

if (!lire_entier(&choix)) {
    printf("Entrée invalide.\n");
    return;
}

if (!lire_char(&cmd)){
    continue;
}

FILE* f = fopen(COUTS_FILE, "r");

if (!f) {
    perror(COUTS_FILE);
    exit(EXIT_FAILURE);
}

void sauvegarder(Partie* p) { //savegarde robuste

    FILE* f = fopen(SAVE_FILE, "wb");

    if (!f) {
        perrorr("Sauvegarde");
        return;
    }

    if (fwrite(p, sizeof(Partie), nr 1, f) != 1) {
        printf("[!] Erreur d'écriture.\n");
    }

    fclose(f);
}

int charger_sauvegarde(Partie* p) { // Chargement robuste

    FILE* f = fopen(SAVE_FILE, "rb");

    if (!f)
        return nr 0;

    if (fread(p, sizeof(Partie), nr1, f) != 1) {
        fclose(f);
        printf("[!] Sauvegarde corrompue.\n");
        return 0;
    }

    fclose(f);
    return 1;
}

char ligne[128];

while (fgets(ligne, sizeof(ligne), f)) {

    int id;
    int cout;

    if (sscanf(ligne, "%d %d", &id, &cout) != 2)
        continue;

    if (id >= 0 && id < NB_INSTRUMENTS)
        couts[id] = cout;
}

if (nb_instr >= 3) {
    printf("Trop d'instruments dans %s\n", PATHO_FILE);
    break;
}

if (pat->plateau.nb_pose >= 3) {
    printf("Plateau plein.\n");
    return;
}

Instrument i = instrument_depuis_nom(token);

if (i == AUCUN_INSTR) {
    printf("Instrument inconnu : %s\n", token);
    continue;
}

void vider_buffer() {

    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

if (scanf("%d", &choix) != 1) {
    vider_buffer();
}

if (remove(SAVE_FILE) != 0) { //supression sauvegarde
    printf("Erreur suppression sauvegarde.\n");
}

