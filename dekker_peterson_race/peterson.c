#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX 10

#define P 0
#define Q 1

_Atomic volatile int needp;
_Atomic volatile int needq;
_Atomic volatile int turn;

void *p(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		usleep(random() % 100000);
		needp=1;
		turn=Q;
		while(needq && turn != P)
			usleep(random() % 1000);
		printf("P IN\n");
		usleep(random() % 100000);
		printf("P OUT\n");
		needp=0;
	}
}

void *q(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		usleep(random() % 200000);
		needq=1;
		turn=P;
		while(needp && turn != Q)
			usleep(random() % 1000);
		printf("Q IN\n");
		usleep(random() % 200000);
		printf("Q OUT\n");
		needq=0;
	}
}

int main(int argc, char *argv[]) {
	srandom(time(NULL));
	pthread_t pp;
	pthread_t pq;
	pthread_create(&pp, NULL, p, NULL);
	pthread_create(&pq, NULL, q, NULL);
	pthread_join(pp, NULL);
	pthread_join(pq, NULL);
}

