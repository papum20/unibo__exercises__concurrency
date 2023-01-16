#include <pthread.h>

#include "semaphore.h"

semaphore s; // semaforo che blocca le somme
semaphore finished; // semaforo che indica che le somme sono state fatte
semaphore critical_section; // sarà utilizzata per decidere chi entra nella critical section

volatile int sumstops = 0;
volatile int result;

void *sumstop(int v) {
    semaphore_P(critical_section);
    sumstops++;
    semaphore_V(critical_section);

    semaphore_P(s);
    printf("summed %d\n", v);
    result += v;
    semaphore_V(finished);
}


int sumgo(void) {
    semaphore_P(critical_section); // così sumstops è costante all'interno di questa section
    result = 0;
    for (int i = 0; i < sumstops; i++) {
        semaphore_V(s); // permetti alla somma di andare
        semaphore_P(finished); // aspetta che la somma sia finita prima di continuare
    }
    sumstops = 0; // reset number of blocked stops.
    semaphore_V(critical_section);
    return result;
}

void *run_sumgo(void) {
    int res = sumgo();
    printf("the result found is %d\n", res);
    printf("the result found is %d\n", result);
}

int main() {
    srand(time(NULL));
    s = semaphore_create(0);
    critical_section = semaphore_create(1);
    finished = semaphore_create(0);

    int n = rand() % 50;
    pthread_t sumg, s[n];

    for (int i = 0; i < n; i++) {
        pthread_create(&s[i], NULL, sumstop, i);
    }
    pthread_create(&sumg, NULL, run_sumgo, NULL);

    for (int i = 0; i < 10; i++) {
        pthread_join(s[i], NULL);
    }

    pthread_join(sumg, NULL);
}