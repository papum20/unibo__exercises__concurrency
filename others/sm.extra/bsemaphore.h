#ifndef BSEMAPHORE_H
#define BSEMAPHORE_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>

struct bsemaphore;
typedef struct bsemaphore *bsemaphore;

bsemaphore bsemaphore_create(long initval);

void bsemaphore_destroy(bsemaphore s);

void bsemaphore_P(bsemaphore s);

void bsemaphore_V(bsemaphore s);

#endif // BSEMAPHORE_H
