/* ===================== MAIN ===================== */

int main(void) {
srand((unsigned)time(NULL));
Partie p;
memset(&p, 0, sizeof(Partie));
 nouvelle_partie(&p);
charger_couts(&p);
charger_pathologies(&p);
int choix = 1;
if (fichier_sauvegarde_existe()) {
afficher_menu_principal();

if (scanf("%d", &choix) != 1) {
    fprintf(stderr, "[!] Erreur de saisie.\n");
    return 1; 
}

if (choix == 3) return 0;
if (choix == 2) {
if (!charger_sauvegarde(&p)) {
printf("[!] Echec du chargement, nouvelle partie.\n");
} else {
printf("[+] Partie chargée !\n");
}
} else {
nouvelle_partie(&p);
}
} else {
nouvelle_partie(&p);
/* Faire arriver un premier patient immédiatement */
faire_arriver_patient(&p);
}

boucle_jeu(&p);
return 0;
}
