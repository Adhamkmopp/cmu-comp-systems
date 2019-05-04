#include "wrappers.h"

int main (int argc, char *argv[], char* envp[]){

pid_t pid;
int status;

if (pid = Fork() == 0){
    Execve("/bin/ls", argv, envp);
}

pid = waitpid(-1, &status, 0);

if (WIFEXITED(status))
    printf("ls exited normally with status %d\n", WEXITSTATUS(status));


/*
    int i;
    printf("Command-line arguments:\n");

    char* columns;




    for (i=0; argv[i] != NULL; i++)
        printf("argv[%2d]: %s\n", i, argv[i]);

    printf("\n");
    printf("Enviromental variables: \n");
    for(i=0; envp[i]!= NULL; i++)
        printf("evnp[%2d]: %s\n", i, envp[i]);
*/
    exit(0);



}