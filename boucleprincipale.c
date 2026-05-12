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
case '5': deplacer(p, -1, 0); break; // gauche 
case '3': deplacer(p, 1, 0); break; // droite 
case '1': deplacer(p, 0, -1); break; // haut 
case '2': deplacer(p, 0, 1); break; //bas 
case 'e': case 'E': action_e(p); break;
//case 's': case 'S': sauvegarder(p); break;
case 'x': case 'X':
/*printf("Voulez-vous sauvegarder avant de quitter ? (o/n) ");
char rep;
scanf(" %c", &rep);
if (rep == 'o' || rep == 'O') sauvegarder(p);*/
p->partie_terminee = 1;
p->score_final = p->argent;
break;
default:
printf("[!] Commande inconnue.\n");
break;
}

p->tours++;
if (p->tours % 3 == 0) gerer_patience(p); //on diminue la patience tous kes 3 tours
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
