#include "wrappers.h"


const unsigned int rodata_variable = 300;
const char *psig = "Segmentation fault";



int main() 
{
    int status, i;
    pid_t pid;
    unsigned int * crash_ref = rodata_variable;

    for (i = 0; i < 5; i++)                       
	if ((pid = Fork()) == 0){
        *crash_ref = 100;
        printf("%d", *crash_ref);
	    exit(100+i);    
    }
                             

   
    while ((pid = waitpid(-1, &status, 0)) > 0) { 
	if (WIFEXITED(status))                    
	    printf("child %d terminated normally with exit status=%d\n",
		   pid, WEXITSTATUS(status));    
	else if (WIFSIGNALED(status)) {
			int sig = WTERMSIG(status);
			char s[50];
			sprintf(s, "child %d terminated by signal %d", pid, sig);
			psignal (sig, s);
		}
    }

    if (errno != ECHILD)                         
	unix_error("waitpid error");

    exit(0);
}
/* $end waitpid1 */