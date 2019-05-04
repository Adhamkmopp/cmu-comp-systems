#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);

