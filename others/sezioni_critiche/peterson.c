#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define bool char
#define true 1
#define false 0

volatile int a = 0;
volatile int b = 0;

volatile int turn = 0;

const int turn_1 = 0;
const int turn_2 = 1;

volatile bool need_1 = false;
volatile bool need_2 = false;

void *thread1()
{
    while (true) {
        need_1 = true;
        turn = turn_1;
        while (need_2 && turn != turn_2);

        // enter critical sec
        a = 1;
        b = 1;
        printf("Thread 1: a = %d, b = %d\n", a, b);

        need_1 = false;
        // exit critical sec
    }
}

void *thread2() {
    while (true) {
        need_2 = true;
        turn = turn_2;
        while (need_1 && turn != turn_1);

        // enter critical sec
        a = 2;
        b = 2;
        printf("Thread 2: a = %d, b = %d\n", a, b);

        need_2 = false;
        // exit critical sec

        for (int i = 0; i < 10000; i++) {
            // rallenta
            // si può notare che anche rallentando questo, l'altro entra nella sezione critica
            // a differenza della soluzione 1, simile a quanto fa dekker.c
        }
    }
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}