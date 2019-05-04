#include "csapp.h"

int readcnt;
sem_t mutex, w, wplus;

void reader(void)
{
    while(1) {
    P(&mutex);
    P(&wplus);
    readcnt++;
    if (readcnt == 1)
        P(&w);
    V(&mutex);

    /* Critical Section */

    P(&mutex);
    readcnt--;

    if(readcnt == 0)
        V(&w);
    V(&wplus);
    V(&mutex);
    
    }
}

void writer(void){
    while(1){
        P(&w);
        P(&wplus);
        /* Critical Section */
        V(&w);
    }
}

