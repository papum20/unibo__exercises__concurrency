/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * implementation of (fair) nbsemaphores
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
#include<nbsemaphore.h>
#include<bsemaphore.h>

struct bsemq {
	bsemaphore waitsem;
	struct bsemq *next;
};

struct nbsemaphore {
	volatile long value;
	bsemaphore mutex;
	struct bsemq *head, *tail;
};

nbsemaphore nbsemaphore_create(long initval) {
	nbsemaphore s = malloc(sizeof(*s));
	if (s) {
		s->value = initval;
		s->mutex = bsemaphore_create(1);
		s->head = s->tail = 0;
	}
	//printf("nbsemaphore_create %p %d\n",s,initval);
	return s;
}

void nbsemaphore_destroy(nbsemaphore s) {
	bsemaphore_destroy(s->mutex);
	free(s);
}

void nbsemaphore_P(nbsemaphore s) {
	bsemaphore_P(s->mutex);
	//printf("nbsemaphore_P %p %d\n",s,s->value);
	if (s->value <= 0) {
		struct bsemq *new = malloc(sizeof(*new));
		new->waitsem = bsemaphore_create(0);
		new->next = NULL;
		if (s->tail == NULL) 
			s->head = new;
		else
			s->tail->next = new;
		s->tail = new;
		bsemaphore_V(s->mutex);
		bsemaphore_P(new->waitsem);
		bsemaphore_destroy(new->waitsem);
		free(new);
	} else {
		s->value--;
		bsemaphore_V(s->mutex);
	}
}

void nbsemaphore_V(nbsemaphore s) {
	struct bsemq *first;
	bsemaphore_P(s->mutex);
	//printf("nbsemaphore_V %p %d\n",s,s->value);
	first = s->head;
	if (first) {
		s->head = first->next;
		if (s->head == NULL)
			s->tail = NULL;
		bsemaphore_V(first->waitsem);
	} else
		s->value++;
	bsemaphore_V(s->mutex);
}
