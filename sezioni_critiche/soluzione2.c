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
        while (in_2);  // do nothing
        in_1 = true;

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
        while (in_1);  // do nothing
        in_2 = true;

        // enter critical sec
        a = 2;
        for (int i = 0; i < 10000000; i++) {
            // si può notare come a volte viene stampata a = 1, b = 2
            // quindi entrano contemporaneamente nella CS
        }  
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