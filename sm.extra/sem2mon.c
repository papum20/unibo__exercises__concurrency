#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<semaphore.h>
#include<semlist.h>
#include<sem2mon.h>

struct smonitor {
	semaphore mutex;
	struct semlist *urgent;
};

struct scondition {
	struct smonitor *smon;
	struct semlist *blocked;
};

smonitor smonitor_create(void) {
	smonitor m = malloc(sizeof(*m));
	if (m) {
		m->mutex = semaphore_create(1);
		m->urgent = NULL;
	}
	return m;
}

void smonitor_destroy(smonitor m) {
	semaphore_destroy(m->mutex);
	free(m);
}

void smonitor_enter(smonitor m) {
	semaphore_P(m->mutex);
}

void smonitor_exit(smonitor m) {
	if (m->urgent) 
		semaphore_V(semlist_pop(&m->urgent));
	else
		semaphore_V(m->mutex);
}

scondition scondition_create(smonitor m) {
	scondition c = malloc(sizeof(*c));
	if (c) {
		c->smon = m;
		c->blocked = NULL;
	}
	return c;
}

void scondition_destroy(scondition c) {
	free(c);
}

void scondition_wait(scondition c) {
	semaphore s = semaphore_create(0);
	semlist_enqueue(&c->blocked, s);
	smonitor_exit(c->smon);
	semaphore_P(s);
	semaphore_destroy(s);
}

void scondition_signal(scondition c) {
	if (c->blocked) {
		semaphore s = semaphore_create(0);
		semlist_push(&c->smon->urgent, s);
		semaphore_V(semlist_dequeue(&c->blocked));
		semaphore_P(s);
		semaphore_destroy(s);
	}
}
