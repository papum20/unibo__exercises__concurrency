//https://github.com/csunibo/sistemi-operativi/blob/main/prove/scritto/scritto-2022-07-20-testo.pdf
//esercizio c1:
/*In un porto con una sola banchina utilizzabile occorre caricare cereali sulle navi. I camion portano i cereali al porto. Una sola nave alla volta può essere attraccata al molo, un solo camion alla volta scarica i cereali nella nave.
Il codice eseguito da ogni nave è:
nave[i] process:
	porto.attracca(capacità)
	porto.salpa()
	...naviga verso la destinazione
il codice di ogni camion è:
camion[j] process:
	while(true):
	quantità = carica_cereali()
	porto.scarica(quantità)
I camion fanno la spola dai depositi alla nave. La nave arriva vuota e può salpare solo se è stata completamente riempita (la somma delle quantità scaricate dai camion raggiunge la capacità indicata come parametro della funzione attracca). Se un camion può scaricare solo parzialmente il suo carico rimane in porto e aspetta di completare l'operazione con la prossima nave che attraccherà al molo.
Scrivere il monitor porto.
*/


//soluzione es c.1 del prof Davoli

/*
monitor porto(){
  boolean is_molo_free = true //per la nave
  boolean is_park_free = true //per il camion
  int dariempire=0 //per la nave, quanto è ancora da riempire?
  condition ok2attracca;
  condition ok2salpa;
  condition ok2scarica;
  condition ok2ancora;

  porto attracca(capacita){
    if (is_molo_free == false)
      ok2attracca.wait() //se il molo è occupato fermati ad aspettare
    is_molo_free = False //attracca.
    dariempire = capacita
    ok2ancora.signal()

  porto.salpa(){
    if (dariempire > 0)
      ok2salpa.wait() //posso salpare solo se ho saturato la capacità della nave
    is_molo_free = True //salpa.

  porto scarica(quantita){
    if (is_park_free == false)
      ok2scarica.wait() //se il parcheggio è occupato fermati ad aspettare
    is_park_free = False //parcheggia.
    while (quantita > dariempire){ //la quantità non può essere versata completamente
      quantita -= dariempire //metti quello che ci sta sulla nave
      dariempire = 0 //satura la nave
      ok2salpa.signal() //lascia andare la nave
      ok2ancora . wait() //il camion deve aspettare la prossima nave
    }
    dariempire -= quantita //metti tutta la capacità del camion sulla nave
    if (dariempire == 0) //ora la nave è riempita?
      ok2salpa.signal()
    is_park_free = True //il camion se ne va
    ok2scarica.signal;
}
*/


//riscritta in linguaggio C per l'esecuzione da Longo Libera

#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>
#include<monitor.h>

volatile bool is_molo_free = true; //per la nave
volatile bool is_park_free = true; //per il camion
volatile int dariempire = 0; //per la nave, quanto è ancora da riempire?

monitor porto;
condition ok2attracca;
condition ok2salpa;
condition ok2scarica;
condition ok2ancora;

void porto_create(void) {
	porto = monitor_create();
	ok2attracca = condition_create(porto);
	ok2salpa = condition_create(porto);
	ok2scarica = condition_create(porto);
	ok2ancora = condition_create(porto);
}

void porto_attracca(int capacita) {
	monitor_enter(porto);
	if (is_molo_free == false)
		condition_wait(ok2attracca);//se il molo è occupato fermati ad aspettare
	is_molo_free = false;//attracca.
	dariempire = capacita;
	condition_signal(ok2ancora);
	monitor_exit(porto);
}

void porto_salpa() {
	monitor_enter(porto);
	if(dariempire > 0)
		condition_wait(ok2salpa); //posso salpare solo se ho saturato la capacità della nave
	is_molo_free = true; //salpa.
	monitor_exit(porto);
}

void porto_scarica(int quantita) {
	monitor_enter(porto);
	if(is_park_free == false)
		condition_wait(ok2scarica); //se il parcheggio è occupato fermati ad aspettare
	is_park_free = false; //parcheggia
    while (quantita > dariempire){ //la quantità non può essere versata completamente
    	quantita -= dariempire; //metti quello che ci sta sulla nave
    	dariempire = 0; //satura la nave
		condition_signal(ok2salpa); //lascia andare la nave
		condition_wait(ok2ancora); //il camion deve aspettare la prossima nave
    }
    dariempire -= quantita; //metti tutta la capacità del camion sulla nave
    if (dariempire == 0) //ora la nave è riempita?
		condition_signal(ok2salpa);
	is_park_free = true; //il camion se ne va
	condition_signal(ok2scarica);
	monitor_exit(porto);
}


void *nave(void* arg) {	
	while (1) {
		printf("\t\tnave ready\n");
		int nave_capienza = random() % 300;
		printf("\t\tcapienza_nave = %d\n", nave_capienza);
		porto_attracca(nave_capienza);
		printf("\t\tattracca\n");
		porto_salpa();
		printf("\t\tsalpa\n");
		printf("\t\t...naviga verso la destinazione...\n");
		usleep(random() % 2000000);
	}
}

void *camion(void* arg) {	
	while (1) {
		printf("camion ready\n");
		usleep(random() % 2000000);		
		int cereali_sul_camion = random() % 300;
		printf("sul_camion = %d\n", cereali_sul_camion);
		porto_scarica(cereali_sul_camion);
		printf("scarica\n");
	}
}


int main(int argc, char *argv[]) {
	pthread_t nave_t;
	pthread_t camion_t;
	porto_create();
	srandom(time(NULL));
	pthread_create(&nave_t, NULL, nave, NULL);
	pthread_create(&camion_t, NULL, camion, NULL);
	pthread_join(nave_t, NULL);
	pthread_join(camion_t, NULL);
}

