#include "cs.h"

volatile bool needp = false, needq = false;

volatile int turn = TURN_1;


void *thread1() {
	while(true) {
		needp = true;
		turn = TURN_2;
		while(turn != TURN_1 && needq)
			usleep(random() % (WAIT_MAX - WAIT_MIN) + WAIT_MIN);
		thread_action(1);
		needp = false;
	}
}

void *thread2() {
	while(true) {
		needq = true;
		turn = TURN_1;
		while(turn != TURN_2 && needp)
			usleep(random() % (WAIT_MAX - WAIT_MIN) + WAIT_MIN);
		thread_action(2);
		needq = false;
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
