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

void *thread1()
{
    while (true) {
        while (turn != turn_1);  // do nothing
        
        // enter critical section
        a = 1;
        b = 1;
        if (a == 1 && b == 1) {
            printf("Thread 1: a = %d, b = %d\n", a, b);
        }
        turn = turn_2;
        // exit critical section
    }
}

void *thread2()
{
    while (true) {
        while (turn != turn_2);  // do nothing

        // enter critical section
        a = 2;
        b = 2;
        if (a == 2 && b == 2) {
            printf("Thread 2: a = %d, b = %d\n", a, b);
        }
        turn = turn_1;
        // exit critical section
        
        for (int i = 0; i < 100000000; i++) {
            // do nothing
            // questo è un esempio di un processo molto lento, che rallenta anche l'altro
        }
    }
}

int main() 
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}