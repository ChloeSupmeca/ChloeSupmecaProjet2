FILE*fichier==NULL;
fichier=fopen("couts.txt", "r");

if (fichier == NULL){
    printf("Ouverture du fichier impossible\n");
    printf("code d'erreur = %d \n", errno );
    printf("Message d'erreur = %s \n", strerror(errno) );
    exit(100);
}



p=fgetc(fichier);

fclose(fichier);
