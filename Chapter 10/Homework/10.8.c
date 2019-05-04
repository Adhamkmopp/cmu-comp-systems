#include "csapp.h"
#define MAXBUFF 12

int main (int argc, char **argv){
struct stat stat;
char *type, *readok;
int fd = atoi(argv[1]);

if (argc!=2 && !isdigit(argv[1])){
    printf("Invalid file descriptor\n");
    exit(-1);
}

Fstat(fd, &stat);

if(S_ISREG(stat.st_mode))
    type = "regular";
else if (S_ISDIR(stat.st_mode))
    type = "directory";
else type = "other";

if ((stat.st_mode & S_IRUSR))
    readok = "yes";
else
    readok = "no";

printf("type: %s, read: %s\n", type, readok);

}
