#ifndef NBSEMAPHORE_H
#define NBSEMAPHORE_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>

struct monsemaphore;
typedef struct monsemaphore *monsemaphore;

monsemaphore monsemaphore_create(long initval);

void monsemaphore_destroy(monsemaphore s);

void monsemaphore_P(monsemaphore s);

void monsemaphore_V(monsemaphore s);

#endif // NBSEMAPHORE_H
