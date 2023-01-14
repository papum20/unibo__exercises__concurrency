#ifndef SEM2MON_H
#define SEM2MON_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<semaphore.h>

struct smonitor;
typedef struct smonitor *smonitor;
struct scondition;
typedef struct scondition *scondition;

smonitor smonitor_create(void);

void smonitor_destroy(smonitor m);

void smonitor_enter(smonitor m);

void smonitor_exit(smonitor m);

scondition scondition_create(smonitor m);

void scondition_destroy(scondition c);

void scondition_wait(scondition c);

void scondition_signal(scondition c);
	
#endif
