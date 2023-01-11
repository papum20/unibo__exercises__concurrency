#ifndef SEMLIST_H
#define SEMLIST_H
#include <semaphore.h>
struct semlist;

void semlist_enqueue(struct semlist **tp, semaphore sem);

void semlist_push(struct semlist **tp, semaphore sem);

semaphore semlist_dequeue(struct semlist **tp);

semaphore semlist_pop(struct semlist **tp);

#endif
