# Chapter 8:Exceptional Control Flow
## Exception Types

An event occurs that causes the processor to make an indirect procedure call through a jump table (exception table but not always such as in the case of syscalls), and transfers control to a system subroutine (exception handlers running in *kernel* mode) that finishes its work and returns control to the current (interrupt) or next instruction (trap) or terminate the program entirely (fault/abort).

## Mechanism of Action

An event index k is triggered by a change in the processor state, and it is used in conjunction with  the *exception table base register* as an index into the exception handler jump table (initialzied on boot by the OS) that contains the address of the proper exception handler code. A return address (this/next instruction) is pushed unto the stack (user or kernel depending) alongwith conditions flags.

## Classes

1.  Interrupts: asyncrhonous from I/O devices. A device identifier (port?) is placed onto the system bus and an interrupt pin is switched on. The processor returns to the next instruction.
2.  Traps: inentional exceptions that call system procedures such as write and exit. A syscall in assembly decodes the argument list and transfers control to the system subroutine (running in kernel mode).
3.  Faults: like page faults, an error that may be corrected and thereby returned to the offending instruction. Also *fatal protection fault* where a program running in user mode attempts a privelaged instruction.
4.  Aborts: smashy smashy

## Exceptions in Linux

Index 0 to 31 is defined by Intel, 32 to 255 are interrupts and traps defined by the OS. Linux/x86 faults and aborts include page faults, segmentation faults, machine faults and division by zero. Linux system calls are done via jump table (*not* the same as an exception table) in the kernel.

Sytems-level functions are really C wrappers to the assembly instruction *syscall*. Parameters are stored in several registers, but the syscall index is stored in rax, and so is the return value. A negative return value in rax ranging from -4095 to -1 indicates an error for *all* system-level functions (in errno).


## Process & Contexts

A process is an instance of a program in its current state (context) which ultimately includes a private address space and independant logical control flow (sequence of program counters). Concurrency occurs when a process is *preempted* (suspended temporarily), its state pushed unto the kernel stack (includes page tables, process tables and a file table) and control is transferred to another process after its state is restored. Different terminologes refer to the same thing such as; time slicing  and multitasking.

A mode bit is set somewhere to differntiate user and kernel modes and can only change by necessity in the case of exceptions. These include context switching where the kernel executes instructions in kernel mode for saving and loading state presumably, before switching back to user mode.

## System Call Error Handling

For system level functions, *errno* is a global variable that is defined on encountering an error. The *strerror* function prints out the specific erorr string.This error format executes the function within an if statement and immediately checks the return value. It is concise and functional.


```c
void unix_error(char *msg) /*error checking functions */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork(void) /*function wrapper for fork(); return 0 to child and child's pid to parent */
{
    pid_t pid;

    if((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}
```


The fork function is uncommon in that it is called once and returns twice (0 to child, pid of child to parent). The address space is duplicated along with the context (including shared files and open files), and the execution of child and parent is concurrent but there is no direct control on the concurrency pattern.

## Parents & Zombies

When a parent reaps a child, it receives the child's exit status from the kernel and the child's virtual memory is freed. If a parent terminates before the child is, the orphan is passed to init as which has a PID of 1; the ultimate ancestor of all processes.


```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *statusp, int options);

/* preempts the calling process until the child on the wait set terminated (default option 0)
if pid >0, then the child is refered to directly. if pid = -1, then all children are included.
Return terminated child pid or -1 if no children exist

WNOHANG: return immediately if the children have not yet terminated with return value zero
WUNTRACED: default. suspend until only when children have both terminated and stopped and return with terminated/stopped child pid
WCONTINUED: suspend until children have terminated, or a stopped process has resumed on receiving a SIGCONT singal
*/

```
*statusp encodes information about the child process when it terminated, which could include the number of the singal that caused it to stop or terminated  or if it even exited normally. Exit status functions are the primary use for this.

## Error Conditions
If the caller has no children, then waitpid returns -1 and sets errno to ECHILD. If there was a signal interruption, waitpid returns -1 and errno is set to EINTR. The code examples below reap children, first in no particular order, then in a controlled manner. The interestring bits are all in the while statement; the while statement is used to continously go back and execute the waitpid function, and also increments of the index within an array index. I like the compaction of statements like these.

```c
while((pid = waitpid (-1, &status, 0)) > 0){
    if (WIFEXITED(status))
        printf("child %d terminated normally with exit status = %d\n", pid, WEXITSTATUS(status));
    else
        printf("child %d terminated abnormally\n", pid);
}

while((retpid = waitpid (pid[i++], &status, 0)) > 0){
    if (WIFEXITED(status))
        printf("child %d terminated normally with exit status = %d\n", pid, WEXITSTATUS(status));
    else
        printf("child %d terminated abnormally\n", pid);
}

```

## Loading & Running (execv)

Execv launches a new *program* within the context of the current process; it overwrites the address space and maintains the same PID, but the file descriptors is kept.

```c
int execv ( const char *filename, const char *argv[], const char *envp[]);
```

Both argv and envp are NULL terminated arrays of pointers to actual string arguments placed at the bottom of the stack. The number of arguments is found in argc (%rdi), while the address of argv (%rsi) and envp (%rdx) point to the first array entries of both. The first element in argv however is the name of the program by convention.

## Signals

Sending a signal, the kernel delivers it but it has to be received by the destination; it can ignore, terminate or catch the signal by executing a user-level signal handler. SIGKILL cannot be caught or ignored. while SIGINT can. Until a signal of type k is received, it is *pending* and the signal mask is set to reflect a blocked signal. Whenever a signal is delivered, it affects all processes running under the same process group which is the parent's pid.

Once returning from kernel mode, the kernel checks unblocked signals and forces one through. Each program then by default either:


1.  terminate process
2.  terminate and dump core
3.  stop until restart by SIGCONT
4.  ignore the signal

The kernel implicitly blocks incoming signals of type k if another preceeding signal is currently being processed. One could also explicitly block incoming signals of a specific type using the *sigprocmask* function and its many useful helpers.


## Signal Handler

```c
#include <signal.h>

typedef void (*sighandler_t)(int);
sighandler_t signal (int signum, sighandler_t handler);

*/
```
This function sets the behaviour of a process when a signal is received. By default, the handler is set to SIG_DFL, but it can also be set to ignore the signal when set to SIG_IGN. Otherwise, a function may be loaded in the handler parameter to handle the signal. On error, the signal function returns SIG_ERR
## Signal Blocking

There are 5 functions used in signal blocking; one for setting the blocked signal using a set as an input and instructions on how to apply the block, and four others for creating and manipulating the set. The sigprocmask function specifically defines the behaviour of the block.

```c

sigset_t mask, prev_mask;

Sigemptyset(&mask);
Sigaddset(&mask, SIGINT);

/* Block SIGINT signals temporarily*/

Sigprocmask(SIG_BLOCK, &mask, &prev_mask)

// Safe code goes here

/* Restore previous block set */
Sigprocmask(SIG_SETMASK, &mask, &prev_mask)
```


## Safe Signal Handling
Signal handlers run concurrently to the main process and to each other, therefore, special guidlines should be followed to allow for proper execution of signal handlers. Tha main issue is that both the handler and the main process share the same address space:

1.  Keep em simple. set global variables and retyurn
2.  Call only async-signal-safe functions: reenetrant functions (purely local variables) and non-singnal interruptable.
3.  Save and restore errno through a local variable
4.  Protect access to shared global data structures: -temporarily- block all sources of signal interrupts while reading or writing to a shared global structure.
5.  Declare global variables with volatile: int volatile g; prohibits the compiler from caching g into a register, and forces a memory read each time it is referenced
6.  Atomics, for Arrakis: use volatine sig_atomic_t flag; instructions. They are uninterruptible (caveat: single instruction use only).

One way of ensuring proper functionality is by making use of the sigsuspend function. It temporarily blocks all signals set in mask and pauses the process until a signal is received that terminates or runs a handler:

```c
sigsuspend(const sigset_t *mask);
```

Which is equivalent to an atomic and simultanous application of:

```c
sigprocmask(SIG_BLOCK, &mask, &prev);
pause();
```

## A word on set and lon jumps..

There isn't much to say here except that setjmp saves the program state (instruction counter, general purpose registers and stack pointer) and longjmp restores it, returning some value. The setjmp function is equivalent to try and catch, and longjmp is throw.