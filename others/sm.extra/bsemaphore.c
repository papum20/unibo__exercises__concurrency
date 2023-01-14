/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * implementation of (fair) bsemaphores
 *        
 * sm is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *             
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *                   
 */                   

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<tlist.h>
#include<bsemaphore.h>
#include<semaphore.h>

#if 0
#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct bsemaphore {
	volatile long value;
	pthread_mutex_t lock;
	struct tlist *q;
};

bsemaphore bsemaphore_create(long initval) {
	bsemaphore s = malloc(sizeof(*s));
	if (s) {
		s->value = !!initval;
		s->q = NULL;
		pthread_mutex_init(&s->lock, NULL);
	}
	return s;
}

void bsemaphore_destroy(bsemaphore s) {
	pthread_mutex_destroy(&s->lock);
	free(s);
}

void bsemaphore_P(bsemaphore s) {
	mutex_in(&s->lock);
	//printf("%p P %d %d\n",s,s->value,tlist_empty(s->q));
	if (s->value <= 0) {
		tlist_enqueue(&s->q, pthread_self());
		mutex_out(&s->lock);
		suspend();
	} else if (tlist_empty(s->q)) {
		if (tlist_empty(s->q))
			s->value--;
		else
			wakeup(tlist_dequeue(&s->q));
		mutex_out(&s->lock);
	}
}

void bsemaphore_V(bsemaphore s) {
	mutex_in(&s->lock);
	//printf("%p V %d %d\n",s,s->value,tlist_empty(s->q));
	if (s->value >= 1) {
		tlist_enqueue(&s->q, pthread_self());
		mutex_out(&s->lock);
		suspend();
	} else {
		if (tlist_empty(s->q))
			s->value++;
		else
			wakeup(tlist_dequeue(&s->q));
		mutex_out(&s->lock);
	}
}
#else
struct bsemaphore {
	semaphore s0,s1;
};

bsemaphore bsemaphore_create(long initval) {
	bsemaphore s = malloc(sizeof(*s));
	s->s0 = semaphore_create(!!initval);
	s->s1 = semaphore_create(!initval);
	return s;
}

void bsemaphore_destroy(bsemaphore s) {
	semaphore_destroy(s->s0);
	semaphore_destroy(s->s1);
	free(s);
}

void bsemaphore_P(bsemaphore s) {
	semaphore_P(s->s0);
	semaphore_V(s->s1);
}

void bsemaphore_V(bsemaphore s) {
	semaphore_P(s->s1);
	semaphore_V(s->s0);
}
#endif
