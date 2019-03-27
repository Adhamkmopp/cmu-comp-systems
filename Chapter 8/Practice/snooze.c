#include "wrappers.h"

void siginthandler(int sig){
    return;
}

void snooze(unsigned int secs){
    unsigned int rc = sleep(secs);
    printf("Slept for %d of %d secs. \n", secs-rc, secs);
    return rc;
}
int main(int argc, char ** argv)
{
    if (argc!=2){
        printf("Wrong number of parameters\n");
        exit(0);
    }
        
    if (signal(SIGINT, siginthandler) == SIG_ERR)
        unix_error("Signal error");

    snooze(atoi(argv[1]));
    
    }
