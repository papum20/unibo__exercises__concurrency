#include "cs.h"

volatile bool inp = false, inq = false;



void *thread1() {
	while(true) {
		inp = true;
		while(inq);
		thread_action(1);
		inp = false;
	}
}

void *thread2() {
	while(true) {
		inq = true;
		while(inp);
		thread_action(2);
		inq = false;
	}
}


int main() {
	srand(time(NULL));
	pthread_t t1, t2; 
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
