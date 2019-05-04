#include "csapp.h"

int wrtrcnt;
sem_t mutex, w, wplus;

void reader(void){
    while(1){
        P(&w);
        P(&wplus);
        /* Critical Section */
        V(&w);
    }
}

void writer(void){
    while(1) {
    P(&mutex);

    wrtrcnt++;
    if (wrtrcnt == 1)
        P(&w);
    V(&mutex);

    /* Critical Section */

    P(&mutex);
    wrtrcnt--;

    if(wrtrcnt == 0)
        V(&w);
    V(&mutex);
    
    }
}
