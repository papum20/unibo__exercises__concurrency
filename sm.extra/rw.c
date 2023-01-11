#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<semaphore.h>
 
#define NUMBER_OF_READERS 3
#define NUMBER_OF_WRITERS 3
semaphore mutex;
semaphore ok2read;
semaphore ok2write;
 
int nr=0;
int nw=0;
int wr=0;
int ww=0;
 
// (nw == 0 && nr == 0) || (nr > 0 && nw == 0) || nw == 1
// (nw == 0 && nr >= 0) || nw == 1
 
void startread(void) {
// <await nw == 0 -> nr++>
    semaphore_P(mutex);
    if (nw + ww > 0) {
        wr++;
        semaphore_V(mutex);
        semaphore_P(ok2read);
        wr--;
    }
    nr++;
    printf("NR %d - NW %d - WR %d - WW %d \n", nr, nw, wr, ww);
    if (wr > 0 )
        semaphore_V(ok2read);
    else
        semaphore_V(mutex);
}
 
void endread(void) {
// <nr-->
    semaphore_P(mutex);
    nr--;
    printf("NR %d - NW %d - WR %d - WW %d \n", nr, nw, wr, ww);
    if (nr == 0 && ww > 0)
        semaphore_V(ok2write);
    else
        semaphore_V(mutex);
}
 
void startwrite(void) {
// <await nw == 0 && nr == 0 -> nw++>
    semaphore_P(mutex);
    if (nw > 0 || nr > 0) {
        ww++;
        semaphore_V(mutex);
        semaphore_P(ok2write);
        ww--;
    }
    nw++;
    printf("NR %d - NW %d - WR %d - WW %d \n", nr, nw, wr, ww);
    semaphore_V(mutex);
}
 
void endwrite(void) {
// <nw-->
    semaphore_P(mutex);
    nw--;
    printf("NR %d - NW %d - WR %d - WW %d \n", nr, nw, wr, ww);
    if (wr > 0)
        semaphore_V(ok2read);
    else if (ww > 0)
        semaphore_V(ok2write);
    else
        semaphore_V(mutex);
}
 
void *reader(void *arg) {
    int i = (uintptr_t)arg;
    while (1) {
        //other code
        usleep(random() % 200000);
        startread();
        //read
        usleep(random() % 200000);
        endread();
    }
}
 
void *writer(void *arg) {
    int i = (uintptr_t)arg;
    while (1) {
        //other code
        usleep(random() % 200000);
        startwrite();
        //write
        usleep(random() % 200000);
        endwrite();
    }
}
 
 
//printf("philo thinking: %d\n",i);
/*while*/
 
int main(int argc, char *argv[]) {
    int i;
    pthread_t t;
    srandom(time(NULL));
    mutex=semaphore_create(1);
    ok2read=semaphore_create(0);
    ok2write=semaphore_create(0);
    for (i=0; i<NUMBER_OF_READERS; i++)
        pthread_create(&t, NULL, reader, (void *)(uintptr_t) i);
    for (i=0; i<NUMBER_OF_WRITERS; i++)
        pthread_create(&t, NULL, writer, (void *)(uintptr_t) i);
    while(1)
        pause();
}
