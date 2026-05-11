void apparitionPatient (Partie*p){

for (int i=0; i<N_FAUTEUILS; i++){
	if(p->patients[i].occupe_fauteuil==0){
		genererPatient(&p->patients[i]);
		return;
		}
	}
}			
