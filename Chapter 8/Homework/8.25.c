#include "wrappers.h"

int timeout = 0;

void alarm_handler(int sig){
    timeout = 1;
}

char* tfgets (char* str[], int n, FILE* stream){
    alarm(5);
    fgets(str, n , stream);
    if(timeout==1){
        return NULL;
    }
    else return str;

}

int main(){

    char *cmdline[128];
    char *cmdline_return[128];

    if (signal(SIGALRM, alarm_handler) == SIG_ERR)
        unix_error("Alarm signal error");

    *cmdline_return = tfgets(cmdline, 128, stdin);

    printf("%s", *cmdline_return);

}