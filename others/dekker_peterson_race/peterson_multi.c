#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define NPROC 5
#define MAX 10

_Atomic volatile int stage[NPROC];
_Atomic volatile int last[NPROC];

void *p(void *arg) {
	int i = (uintptr_t) arg;
	int j;
	int k;
	int n;
	//printf("%d\n", i);
	for (n = 0; n < MAX; n++) {
		usleep(random() % 100000);
		/* Entry protocol */
		for (int j = 1; j <= NPROC; j++) {
			//printf("J %d %d\n", i, j);
			stage[i] = j; last[j] = i;
			for (int k = 1; k <= NPROC; k++) {
				//printf("K %d %d %d\n", i, j, k);
				if (i != k)
					while (stage[k] >= stage[i] && last[j] == i) {
						usleep(random() % 1000);
						//printf("W %d %d %d %d %d %d\n", i,j,k, stage[k], stage[i], last[j]);
					}
			}
		}
		printf("P%d IN (%d)\n", i, n);
		usleep(random() % 100000);
		printf("P%d OUT (%d)\n", i, n);
		/* Exit protocol */
		stage[i] = 0;
	}
}

int main(int argc, char *argv[]) {
	srandom(time(NULL));
	uintptr_t i;
	pthread_t pp[NPROC];
	for (i = 0; i < NPROC; i++)
		pthread_create(&pp[i], NULL, p, (void *) (i + 1));
	for (i = 0; i < NPROC; i++)
		pthread_join(pp[i], NULL);
}

