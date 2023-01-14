#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX 10

#define P 0
#define Q 1

_Atomic volatile int turn;

void *p(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		while (turn == Q)
			;
		printf("P IN\n");
		usleep(random() % 200000 + 1000000);
		printf("P OUT\n");
		turn = Q;
	}
}

void *q(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		while (turn == P)
			;
		printf("Q IN\n");
		usleep(random() % 200000);
		printf("Q OUT\n");
		turn = P;
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

