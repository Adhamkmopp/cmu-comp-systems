#include "csapp.h"

int readcnt;
sem_t mutex, w, wplus;

void reader(void)
{
    while(1) {
    P(&mutex);
    readcnt++;
    
    P(&w);
    /* Critical Section */

    P(&mutex);
    readcnt--;

    V(&w);
    V(&mutex);
    
    }
}

void writer(void){
    while(1){
        P(&w);
        /* Critical Section */
        V(&w);
    }
}

