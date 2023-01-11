#ifndef NBSEMAPHORE_H
#define NBSEMAPHORE_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>

struct nbsemaphore;
typedef struct nbsemaphore *nbsemaphore;

nbsemaphore nbsemaphore_create(long initval);

void nbsemaphore_destroy(nbsemaphore s);

void nbsemaphore_P(nbsemaphore s);

void nbsemaphore_V(nbsemaphore s);

#endif // NBSEMAPHORE_H
