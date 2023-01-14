#include "cs.h"


void thread_action(int thread) {
	for(int i = 0; i < thread * TABS_N; i++) printf("\t");
	printf("in %d\n", thread);

	usleep(random() % WAIT_MIN + WAIT_MAX);

	for(int i = 0; i < thread * TABS_N; i++) printf("\t");
	printf("out %d\n", thread);
}

void print_action(int thread) {
	for(int i = 0; i < thread * TABS_N; i++) printf("\t");
	printf("thread %d\n", thread);
}
