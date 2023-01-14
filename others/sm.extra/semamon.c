#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<tlist.h>
#include<monitor.h>
#include<semamon.h>

struct monsemaphore {
	volatile long value;
	monitor mon;
	condition ok2go;
};

monsemaphore monsemaphore_create(long initval) {
	monsemaphore s = malloc(sizeof(*s));
	s->value = initval;
	s->mon = monitor_create();
	s->ok2go = condition_create(s->mon);
}

void monsemaphore_destroy(monsemaphore s) {
	condition_destroy(s->ok2go);
	monitor_destroy(s->mon);
	free(s);
}

void monsemaphore_P(monsemaphore s) {
	monitor_enter(s->mon);
	if (s->value <= 0)
		condition_wait(s->ok2go);
	s->value--;
	monitor_exit(s->mon);
}

void monsemaphore_V(monsemaphore s) {
	monitor_enter(s->mon);
	s->value++;
	condition_signal(s->ok2go);
	monitor_exit(s->mon);
}


