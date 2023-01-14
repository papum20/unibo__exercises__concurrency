#include "cs.h"

volatile bool needp = false, needq = false;

volatile int turn = TURN_1;


void *thread1() {
	while(true) {
		needp = true;
		while(needq) {
			if(turn == TURN_2) {
				needp = false;
				while(turn == TURN_2);
				needp = true;
			}
		}
		thread_action(1);
		needp = false;
		turn = TURN_2;
	}
}

void *thread2() {
	while(true) {
		needq = true;
		while(needp) {
			if(turn == TURN_1) {
				needq = false;
				while(turn == TURN_1);
				needq = true;
			}
		}
		thread_action(2);
		needq = false;
		turn = TURN_1;
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
