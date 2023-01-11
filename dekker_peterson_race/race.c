/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * producer consumer
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

#define MAX 100000

/*_Atomic*/ volatile int buffer = 0;

void *proc(void *arg) {
	int i;
	for (i = 0; i < MAX; i++) 
		buffer += 1;
}

int main(int argc, char *argv[]) {
	pthread_t p1;
	pthread_t p2;
	pthread_create(&p1, NULL, proc, NULL);
	pthread_create(&p2, NULL, proc, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("SUM = %d\n", buffer);
}

