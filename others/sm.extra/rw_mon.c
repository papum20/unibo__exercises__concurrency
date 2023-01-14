#include<stdio.h>
#include<pthread.h>
#include<stdint.h>
#include<monitor.h>

#define NUMBER_OF_READERS 3
#define NUMBER_OF_WRITERS 3

monitor rwmon;
condition ok2read;
condition ok2write;
int nr=0;
int nw=0;
int ww=0;

void rwmon_create(void) {
	rwmon = monitor_create();
	ok2read = condition_create(rwmon);
	ok2write = condition_create(rwmon);
}

void startread(void) {
	monitor_enter(rwmon);
	if (nw + ww > 0)
		condition_wait(ok2read);
	nr++;
	printf("NR %d - NW %d - WW %d \n", nr, nw, ww);
	condition_signal(ok2read);
	monitor_exit(rwmon);
}

void endread(void) {
	monitor_enter(rwmon);
	nr--;
	printf("NR %d - NW %d - WW %d \n", nr, nw, ww);
	if (nr == 0)
		condition_signal(ok2write);
	monitor_exit(rwmon);
}

void startwrite(void) {
	monitor_enter(rwmon);
	ww++;
	if (nr + nw > 0)
		condition_wait(ok2write);
	ww--;
	nw++;
	printf("NR %d - NW %d - WW %d \n", nr, nw, ww);
	monitor_exit(rwmon);
}

void endwrite(void) {
	monitor_enter(rwmon);
	nw--;
	printf("NR %d - NW %d - WW %d \n", nr, nw, ww);
	condition_signal(ok2read);
	if (nr == 0)
		condition_signal(ok2write);
	monitor_exit(rwmon);
}

void *reader(void *arg) {
	int i = (uintptr_t)arg;
	while (1) {
		//other code
		usleep(random() % 200000);
		startread();
		//read
		usleep(random() % 200000);
		endread();
	}
}

void *writer(void *arg) {
	int i = (uintptr_t)arg;
	while (1) {
		//other code
		usleep(random() % 200000);
		startwrite();
		//write
		usleep(random() % 200000);
		endwrite();
	}
}

int main(int argc, char *argv[]) {
	int i;
	pthread_t t;
	srandom(time(NULL));
	rwmon_create();
	for (i=0; i<NUMBER_OF_READERS; i++)
		pthread_create(&t, NULL, reader, (void *)(uintptr_t) i);
	for (i=0; i<NUMBER_OF_WRITERS; i++)
		pthread_create(&t, NULL, writer, (void *)(uintptr_t) i);
	while(1)
		pause();
}

