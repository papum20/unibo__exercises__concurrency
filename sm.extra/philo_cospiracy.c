#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<semaphore.h>

semaphore philowait[5];
semaphore mutex;
semaphore cospsem;
char philo_status[]="TTTTT";

void baton(int i) {
        int j;
        for (j=1; j<5; j++) {
                if (philo_status[(i+j)%5] == 'W' &&
                        philo_status[(i+j+1)%5] != 'E' && philo_status[(i+j+4) % 5] != 'E') {
                        semaphore_V(philowait[(i+j)%5]);
                        return;
                }
        }
        semaphore_V(mutex);
}

void ok2eat(int i) {
        semaphore_P(mutex);
        /*while*/
        philo_status[i] = 'W';
        if (philo_status[(i+1) % 5] == 'E' ||
                        philo_status[(i+4) % 5] == 'E') {
                semaphore_V(mutex);
                semaphore_P(philowait[i]);
        }
        philo_status[i] = 'E';
        printf("philo eating:   %d |%s|\n",i,philo_status);
        baton(i);
}

void ok2think(int i) {
        semaphore_P(mutex);
        philo_status[i] = 'T';
        printf("philo thinking: %d |%s|\n",i,philo_status);
        baton(i);
}

void cospiracy_in(int i) {
static int firsttime=1;
        semaphore_P(mutex);
        if (firsttime)
                firsttime=0;
        else
                semaphore_V(cospsem);
        semaphore_V(mutex);
}

void cospiracy_out(int i) {
        semaphore_P(cospsem);
}

void *philo(void *arg) {
        int i = (uintptr_t)arg;
        while (1) {
                //think
                usleep(random() % 200000);
                ok2eat(i);
                if (i==1 || i==3) cospiracy_in(i);
                //eat
                if (i==1 || i==3) cospiracy_out(i);
                usleep(random() % 200000);
                ok2think(i);
        }
}


//printf("philo thinking: %d\n",i);
/*while*/ 

int main(int argc, char *argv[]) {
        int i;
        pthread_t philo_t[5];
        srandom(time(NULL));
        mutex=semaphore_create(1);
        cospsem=semaphore_create(0);
        for (i=0; i<5; i++)
                philowait[i]=semaphore_create(0);
        for (i=0; i<5; i++)
                pthread_create(&philo_t[i], NULL, philo, (void *)(uintptr_t) i);
        while(1)
                pause();
}
