#include "cs.h"

volatile int turn = 0;

const int TURN_1 = 0;
const int TURN_2 = 1;


void *thread1() {
	while(true) {
		while(turn != TURN_1);	//wait
		//enter cs
		turn = TURN_1;
		thread_action(1);
		turn = TURN_2;
		//exit cs
	}
}

void *thread2() {
	while(true) {
		while(turn != TURN_2);	//wait
		//enter cs
		turn = TURN_2;
		thread_action(2);
		turn = TURN_1;
		//exit cs
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
