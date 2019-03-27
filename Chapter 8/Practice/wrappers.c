#include "wrappers.h"

void unix_error(char *msg){
    fprintf (stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void){
    pid_t pid;
    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

pid_t Waitpid(pid_t pid, int statusptr, int options){
    pid_t retpid;
    if ((retpid = Waitpid(pid, statusptr, options)) < 0)
        unix_error("Waitpid error");
    return retpid;
}

unsigned int Sleep(unsigned int secs) 
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
	unix_error("Sleep error");
    return rc;
}

void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
    if (execve(filename, argv, envp) < 0)
	unix_error("Execve error");
}

char *Fgets(char *ptr, int n, FILE *stream) 
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
	unix_error("Fgets error");

    return rptr;
}

void Kill(pid_t pid, int sig){
    /* pid < 0: kill every process in the process group |pid|
       pid > 0: kill pid process
       pid = 0: kill every process in the calling process' group including the calling process itself 
       SIGKILL default sig, but what about the others if any?
       */

    int rc;
    if (rc = kill(pid, sig) < 0 )
        unix_error("Kill error");
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
	unix_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
	unix_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{ 
    if (sigfillset(set) < 0)
	unix_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
	unix_error("Sigaddset error");
    return;
}

void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
	unix_error("Sigdelset error");
    return;
}

int Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
	unix_error("Sigismember error");
    return rc;
}
static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}

ssize_t sio_puts (char s[]){
    /* writes to file and retruns number of bytes successfuly written */
    return write (STDOUT_FILENO, s , sio_strlen(s));

}