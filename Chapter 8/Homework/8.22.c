#include "wrappers.h"


int mysystem(char *command[]){
    
    Execve("/bin/sh", &command[0], NULL);

}

int main (int argc, char *argv[]){

pid_t pid;
int status;


if (pid = Fork() == 0){

    mysystem(argv);
}

pid = waitpid(-1, &status, 0);



if (WIFEXITED(status))
    printf("%s exited normally with status %d\n", argv[2], WEXITSTATUS(status));
else 
    printf("%s exited abnormally with signal %d\n", argv[2], WTERMSIG(status));

exit(0);


}