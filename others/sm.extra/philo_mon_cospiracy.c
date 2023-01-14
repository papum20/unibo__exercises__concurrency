#include <stdio.h>
#include <monitor.h>
#include <pthread.h>
#include <stdint.h>

monitor dp;
condition philo_wait[5]; /* status[np(i,1)] == 'T' && status[np(i,-1)] == 'T' */
char status[] = "TTTTT";

int np(int i, int off) {
	return (i+5+off) % 5;
}
	

void dp_create(void) {
	int i;
	dp = monitor_create();
	for (i=0; i<5; i++)
		philo_wait[i] = condition_create(dp);
}

void dp_starteat(int i) {
	monitor_enter(dp);
	if (status[np(i,1)] != 'T' || status[np(i,-1)] != 'T') 
		condition_wait(philo_wait[i]);
	status[i] = 'E';
	printf("%s\n",status);
	monitor_exit(dp);
}

void dp_endeat(int i) {
	monitor_enter(dp);
	status[i] = 'T';
	printf("%s\n",status);
	if (status[np(i,2)] == 'T')
		condition_signal(philo_wait[np(i,1)]);
	if (status[np(i,-2)] == 'T')
		condition_signal(philo_wait[np(i,-1)]);
	monitor_exit(dp);
}

monitor cospy;
condition wait2eat;
int cospy_count;

void cospy_create(void) {
	cospy = monitor_create();
	wait2eat = condition_create(cospy);
}

void cospy_starteat(void) {
	monitor_enter(cospy);
	cospy_count++; 
	condition_signal(wait2eat);
	monitor_exit(cospy);
}

void cospy_endeat(void) {
	monitor_enter(cospy);
	cospy_count--; 
	if (cospy_count == 0)
		condition_wait(wait2eat);
	monitor_exit(cospy);
}

void *philo(void *arg) {
	int i = (uintptr_t) arg;
	while (1) {
		usleep(random() % 2000000);
		dp_starteat(i);
		if (i == 1 || i == 3)
			cospy_starteat();
		usleep(random() % 2000000);
		if (i == 1 || i == 3)
			cospy_endeat();
		dp_endeat(i);
	}
}

int main(int argc, char *argv[]) {
	pthread_t philo_t[5];
	uintptr_t i;
	dp_create();
	cospy_create();
	srandom(time(NULL));
	for (i=0; i<5; i++)
		pthread_create(&philo_t[i], NULL, philo, (void *) i );
	while(1)
		pause();
}

