void majPatient (Partie*p){
	for (int i=0; i<N_FAUTEUILS; i++){
		if (p->patient[i].occupe_fauteuil){
			p->patient[i].patience--;
		}
		if (p->patient[i].patience<=0){
			printf("Patient mécontent parti !");
			p->patient[i].occupe_fauteuil=0;
			p->patient_mecontent++;
		}
	}
}				 	
