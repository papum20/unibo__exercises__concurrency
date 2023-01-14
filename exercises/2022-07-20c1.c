int da_riempire = 0;
bool is_molo_free = true;
bool is_park_free = true;
condition ok2attracca;
condition ok2salpa;
condition ok2scarica;
condition ok2ancora;

porto.attracca(int capacita) {
	if(!is_molo_free)
		ok2attracca.wait();
	is_molo_free = false;
	da_riempire = capacita;
	ok2ancora.signal();
}

porto.salpa() {
	if(da_riempire > 0)
		ok2salpa.wait();
	ok2attracca.signal();
}

porto.scarica(quantita) {
	if(!is_park_free) {
		
}
