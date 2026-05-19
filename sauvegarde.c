#inlcude"structures_projet_dentiste.h"

/* ===================== SAUVEGARDE ===================== */

void sauvegarder(Partie* p) {
if (p == NULL) return; 
FILE* f = fopen(SAVE_FILE, "wb");
if (f == NULL) {
        fprintf(stderr, "[!] Erreur ouverture fichier sauvegarde.\n");
        return;
    }

if (fwrite(p, sizeof(Partie), 1, f) != 1) {
        fprintf(stderr, "[!] Erreur écriture sauvegarde.\n");
    }
fclose(f);
f = NULL;
printf("[+] Partie sauvegardée.\n");
}

int charger_sauvegarde(Partie* p) {
 if (p == NULL) return 0;
FILE* f = fopen(SAVE_FILE, "rb");
if (f == NULL) {
        fprintf(stderr, "[!] Erreur ouverture fichier sauvegarde.\n");
        return 0;
    }

if (fread(p, sizeof(Partie), 1, f) != 1) {
    fprintf(stderr, "[!] Erreur lecture sauvegarde.\n");
    fclose(f);
    return 0;
}
fclose(f);
f = NULL;
return 1;
}

int fichier_sauvegarde_existe() {
FILE* f = fopen(SAVE_FILE, "rb");
if (f) { fclose(f); return 1; }
return 0;
}
