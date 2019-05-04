#include "csapp.h"
#define MAXBUFF 12

int main (int argc, char **argv){
    int n;
    rio_t rio;
    char buf[MAXBUF]; /* interstingly the buffer size should match the MAXBUFF passed in the function but there's no check anywhere if they're equal */

    Rio_readinitb (&rio, STDIN_FILENO);

    while((n= Rio_readnb(&rio, buf, MAXBUFF))!=0){
                Rio_writen(STDOUT_FILENO, buf,n);
            }
       
}