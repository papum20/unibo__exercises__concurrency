semaphore sem = semaphore(0);
semaphore mutex = semaphore(1);
int waiting_put = 0;
int waiting_get = 0;
queue<T> buffer;


void put(T value) {
	mutex.P();
	buffer.push(value);
	if(waiting_get > 0) {
		for(; waiting_get > 0; waiting_get--) sem.V();
		//non sblocca mutex: lo farà l'ultima get a uscire
	} else {
		waiting_put++;
		mutex.V();		//prima di bloccarsi sblocca il mutex per gli altri
		sem.P();
	}
}


list<T> get(void) {
	mutex.P();
	if(waiting_put > 0) {
		queue<T> tmp = buffer.copy();
		for(; waiting_put > 0; waiting_put--) sem.V();
		buffer.clear();
		mutex.V();
		return tmp;
	} else {
		waiting_get++;
		mutex.V();	//same
		sem.P();
		//quando risvegliato da 1 put
		queue<T> tmp = buffer.copy();
		waiting_get--;
		//solo l'ultimo get sbloccato resetta i valori e sblocca il mutex, così gli altri hanno il tempo di usare il buffer e le variabili condivise e di uscire
		if(waiting_get == 0) {
			buffer.clear();
			mutex.V();
		}
		return tmp;
	}
}
