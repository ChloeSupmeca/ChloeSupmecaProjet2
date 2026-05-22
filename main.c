#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "partie.h"
#include "jeu.h"

int main(void) {
    srand((unsigned)time(NULL));

    Partie p;
    memset(&p, 0, sizeof(Partie));
    nouvelle_partie(&p);
    charger_couts(&p);
    charger_pathologies(&p);

    if (fichier_sauvegarde_existe()) {
        afficher_menu_principal();
        int choix = 1;
        if (scanf("%d", &choix) != 1) {
            fprintf(stderr, "[!] Erreur de saisie.\n");
            return 1;
        }

        if (choix == 3) return 0;

        if (choix == 2) {
            if (!charger_sauvegarde(&p)) {
                printf("[!] Échec du chargement, nouvelle partie.\n");
                /* Les pathologies/couts ont déjà été chargés */
                faire_arriver_patient(&p);
            } else {
                /* Recharger les fichiers (pointeurs non sauvegardés) */
                charger_couts(&p);
                charger_pathologies(&p);
                printf("[+] Partie chargée !\n");
            }
        } else {
            /* Nouvelle partie */
            remove(SAVE_FILE);
            faire_arriver_patient(&p);
        }
    } else {
        faire_arriver_patient(&p);
    }

    boucle_jeu(&p);
    return 0;
}
