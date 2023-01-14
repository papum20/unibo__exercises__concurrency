#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define bool char
#define true 1
#define false 0

#define TURN_1 0
#define TURN_2 1

#define TABS_N 1
#define WAIT_MIN 10 	//(2000*1000)
#define WAIT_MAX 10000 //(300*1000)

void thread_action(int thread);
void print_action(int thread);
