#include "csapp.h"


void *thread(void *vargp);

int main(int argc, char **argv){

    pthread_t tid;
    int n = atoi(argv[1]);


    for(int i = 0; i < n; i++)
    Pthread_create(&tid, NULL, thread, NULL);
    Pthread_exit(NULL);
}

void *thread(void *vargp){
    //pthread_detach(pthread_self());
    printf("Hello from thread: %d\n", pthread_self());

}

