#include<stdio.h>
#include<pthread.h>
#include<stdint.h>
#include<monitor.h>

monitor philomon;
condition okstick[5];
int busystick[5];
char status[] = "TTTTT";

void philomon_create(void) {
	int i;
	philomon = monitor_create();
	for (i = 0; i < 5; i++) 
		okstick[i] = condition_create(philomon);
}

void philo_starteat(int i) {
	int sx = i;
	int dx = (i + 1) % 5;
	monitor_enter(philomon);
	status[i] = 'W';
	if (busystick[sx])
		condition_wait(okstick[sx]);
	busystick[sx] = 1;
	if (busystick[dx])
		condition_wait(okstick[dx]);
	busystick[dx] = 1;
	status[i] = 'E';
	printf("%s\n", status);
	monitor_exit(philomon);
}

void philo_endeat(int i) {
	int sx = i;
	int dx = (i + 1) % 5;
	monitor_enter(philomon);
	status[i] = 'T';
	printf("%s\n", status);
	busystick[sx] = 0;
	busystick[dx] = 0;
	condition_signal(okstick[sx]);
	condition_signal(okstick[dx]);
	monitor_exit(philomon);
} 

void *philo(void *arg) {
	int i = (uintptr_t)arg;
	while (1) {
		usleep(random() % 200000);
		philo_starteat(i);
		usleep(random() % 200000);
		philo_endeat(i);
	}
}

int main(int argc, char *argv[]) {
	int i;
	pthread_t philo_t[5];
	srandom(time(NULL));
	philomon_create();
	for (i=0; i<5; i++)
		pthread_create(&philo_t[i], NULL, philo, (void *)(uintptr_t) i);
	for (i=0; i<5; i++)
		pthread_join(philo_t[i], NULL);
}
