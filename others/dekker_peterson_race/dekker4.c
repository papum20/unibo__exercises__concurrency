#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX 10

#define P 0
#define Q 1

_Atomic volatile int inp;
_Atomic volatile int inq;

void *p(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		//usleep(random() % 1000);
		inp=1;
		while(inq) {
			inp=0;
			usleep(random() % 1000);
			inp=1;
		}
		printf("P IN\n");
		usleep(random() % 200000);
		printf("P OUT\n");
		inp=0;
	}
}

void *q(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) {
		//usleep(random() % 1000);
		inq=1;
		while(inp) {
			inq=0;
			usleep(random() % 1000);
			inq=1;
		}
		printf("Q IN\n");
		usleep(random() % 200000);
		printf("Q OUT\n");
		inq=0;
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

