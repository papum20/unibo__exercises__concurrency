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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define MAX 100000

void *proc(int *buf) {
	int i;
	for (i = 0; i < MAX; i++) 
		*buf += 1;
}

void *procm(int *buf) {
	int i;
	for (i = 0; i < MAX; i++) 
		*buf -= 1;
}

int main(int argc, char *argv[]) {
	int *newbuf = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	int status;
	pid_t p;

	*newbuf = 0;
	switch (p = fork()) {
		case 0:
			procm(newbuf);
			return 0;
		default:
			proc(newbuf);
			break;
		case -1: return 2;
	}

	waitpid(p, &status, 0);
	printf("SUM = %d\n", *newbuf);
}

