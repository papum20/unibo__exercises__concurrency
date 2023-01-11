/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *      
 * Dining Philosophers (this source code can generate deadlocks!)
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
#include<stdint.h>
#include<pthread.h>
#include<semaphore.h>

semaphore stick[5];
char philo_status[]="TTTTT";

void *philo(void *arg) {
	int i = (uintptr_t)arg;
	printf("philo thinking: %d\n",i);
	while (1) {
		usleep(random() % 200000);
		semaphore_P(stick[i]);
		//usleep(100000);
		semaphore_P(stick[(i+1)%5]);
		philo_status[i] = 'E';
		printf("philo eating:   %d |%s|\n",i,philo_status);
		usleep(random() % 200000);
		philo_status[i] = 'T';
		printf("philo thinking: %d |%s|\n",i,philo_status);
		semaphore_V(stick[i]);
		semaphore_V(stick[(i+1)%5]);
	}
}

int main(int argc, char *argv[]) {
	int i;
	pthread_t philo_t[5];
	srandom(time(NULL));
	for (i=0; i<5; i++)
		stick[i]=semaphore_create(1);
	for (i=0; i<5; i++)
		pthread_create(&philo_t[i], NULL, philo, (void *)(uintptr_t) i);
	for (i=0; i<5; i++)
		pthread_join(philo_t[i], NULL);
}
