void fin_de_Partie(Partie *p){
   int x=0;
	int y=0;
   int occupe;
   occupe=presence_patient(p, x, y);
	for (int i=0; i<N_FAUTEUILS; i++){
      p->patients[i].occupe_fauteuil;
      if((occupe==1) && (p->patients_mecontents==1)){
            printf("Fin de la partie ! \n");
      }
   }
}
