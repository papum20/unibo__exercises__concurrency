#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define bool char
#define true 1
#define false 0

volatile int a = 0;
volatile int b = 0;

volatile int turn = 0;

volatile bool in_1 = false;
volatile bool in_2 = false;

void *thread1()
{
    while (true) {
        in_1 = true;
        while (in_2);  // do nothing

        // enter critical sec
        a = 1;
        b = 1;
        printf("Thread 1: a = %d, b = %d\n", a, b);

        in_1 = false;
        // exit critical sec
    }
}

void *thread2()
{
    while (true) {
        in_2 = true;
        while (in_1);  // do nothing

        // enter critical sec
        a = 2;
        b = 2;

        printf("Thread 2: a = %d, b = %d\n", a, b);

        in_2 = false;
        // exit critical sec

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