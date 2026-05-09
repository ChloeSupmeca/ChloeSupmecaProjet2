void majPatient (Partie*p){
	for (int i=0; i<N_FAUTEUILS; i++){
		if (p->patients[i].occupe_fauteuil){
			p->patients[i].patience--;
		}
		if (p->patients[i].patience<=0){
			printf("Patient mécontent parti !");
			p->patients[i].occupe_fauteuil=0;
			p->patients_mecontents++;
		}
	}
}				 	
