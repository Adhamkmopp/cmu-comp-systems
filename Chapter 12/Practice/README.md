# Chapter 12: Concurrent Programming

## Concurrency Models
There are three main concurrency models, each with its own merits and drawbacks and not all are interchangable: I/O multiplexing for example is very specific to file descriptor inputs and choices are locked in once a file descriptor is ready. Logical flows run within a single context, and schedueling is set expicitly by the application. A process based approach on the other hand, is based on creating child processes - an expensive, and inefficient operation- with its own virtual memory space where logical flows are scheudeled by the kernel. Lastly, a thread based approach addresses the drawbacks of both methods while retaining their respective merits; scheudeling is taken over by the kernel, but a single address space is common between all threads.

### Process based


Fork and exec, as the example below shows. A child process is forked and concurrency/context switcing is left up to the CPU.

~~~c

/* 
 * echoserverp.c - A concurrent echo server based on processes
 */ 
/* $begin echoserverpmain */
#include "csapp.h"
void echo(int connfd);

void sigchld_handler(int sig) //line:conc:echoserverp:handlerstart
{
    while (waitpid(-1, 0, WNOHANG) > 0)
	;
    return;
} //line:conc:echoserverp:handlerend

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(struct sockaddr_storage); 
	connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
	if (Fork() == 0) { 
	    Close(listenfd); /* Child closes its listening socket */
	    echo(connfd);    /* Child services client */ //line:conc:echoserverp:echofun
	    Close(connfd);   /* Child closes connection with client */ //line:conc:echoserverp:childclose
	    exit(0);         /* Child exits */
	}
	Close(connfd); /* Parent closes connected socket (important!) */ //line:conc:echoserverp:parentclose
    }
}
/* $end echoserverpmain */
~~~

###  I/O Mulitplexing: 

A timeout function suspends the process until one or more file descriptors are ready.

~~~c
#inlcude <sys/select.h>

int select(int n, fd_set *fdset, NULL, NULL, NULL);

FD_ZERO(fd_set *fdset);
FD_CLR(int *fd, fd_set *fdset);
FD_SET(int *fd, fd_set *fdset);
FD_ISSET(int *fd, fd_set *fdset);
/* FD_SETSIZE is the maximum number of available file descriptors */
~~~

The descriptor sets *fdset* is a bit vector, much like the signal sets from previously. In the case of reading, the select function returns a modified read set called the *ready* set (no room for confusion there, not at all) with the FD that can accept a request for a single byte. One major drawback is that once a choice is ready to be made, that process is locked into that choice until it otherwise terminates.

One way around that issue is by implementing a finer granuality ("Event Driven" modification), which increases code complexity but allows for *logical flow* switching within the same process. A major advantage is the maintainance of one and only process which leads to easier sharing of data,easier debugging, prioritizing *logical flows*, and all without expensive context switches. On the other hand, multi-core processors are under-utilized.

## Concurrency With Threads

Threads are logical flows within the same process, running concurrently and schedueled by the kernel. It is a hybrid of the above two methods. Each thread has its own unique integer ID, stack, stack pointer, program counter and general purpose registers despite sharing the same virtual address space including the heap. In essence, a child process within the same context as before but with a common address space.

Threads begin as a main thread that creats a pool of *peer* thread, with the right to kill other threads or wait till termination. There is no strict parent/child hierarchy. Context switches also apply to threads but is much faster given that it's much smaller than a process context. They also share the same data. In other words, threads are just independant run functions much like forking a process but are less expensive, and do share the same virtal address space. It has nothing to do with being inherently parallel or *"multi-core"*. Parellalism is a subset of concurrency.

### POSIX Thread

Pthread is a collection of APIs of over 60 functions that is the standard for manipulating and maintaining threads in C programs.They act as children still and will need to be reaped by the main thread.
The create function below creates a new thread, sets the tid object to the new thread ID and starts the thread in its own, new context by running the associated function. The function takes a single *args* pointer. These are the arguments passed to the function itself, and if the need arises that more than one argument need to be passed, then it should be placed in a struct or some other type of object and passed as a pointer. Return values are actually pointers that can be collected by the pthread_join function at the time of reaping terminated threads.

~~~c
typedef void *(func)(void *);
int pthread_create(pthrea_t *tid, pthread_attr_t *attr, func *f, void *arg);

pthread_t pthread_self(void); /* which thread am I ? */
~~~

Likewise, terminating a thread is done through the pthread_exit or pthread_cancel functions with differing behaviour depending on whether the main thread or one of the peer threads made the call. Termination can also be done implictly when the main thread simply returns, or calls to the unix exit function from *any* peer thread that leads to the termination of the entire process.

~~~c
#include <pthread.h>
void pthread_exit (void *thread_return); /* terminates all threads and then itself and the process.
                                          * returns the thread return value
                                          */
int pthread_cancel(pthread_t tid);        /* peer thread termiantes itself, so long as the tid is correct */

~~~

Threads also need to be reaped after termination. The pthread_join function stores the generic (void *) pointer returned by the thread routine in thread_return and blocks the main thread until thread termination by specific ID. Meaning, it cannot wait for any arbitrary thread to temrinate. The thread in question has to be waited on explicitly, which avoids all the shenanigins caused by the equivalent wait() function in process handling.

~~~c
int pthread_join(pthread_t tid, void **thread_return);
~~~

### Detaching Threads

Threads are joinable by default, meaning they can be killed and indeed have to be reaped by other threads. Detached threads are non-killable, but their memory resources are reaped automatically on termination which can be convenient in some cases. The function detaches a joinable thread, and can be called on itself with the argument pthread_self().

~~~c
int pthread_detach(pthread_t id);
~~~

### Initializing Threads

The virtual memory space is shared between all threads, and not all thread stacks are protected, which segregates variables into 3 distinct classes:

1.  Global variables: instanced only once, shared by all threads.
2.  Local automatic: variables decalred inside thread routines without the static attribute. Each thread gets its own local copy on its own stack.
3.  Local static: static variables are declared once and shared by all peer threads, much like a global variable, but invisible to the main thread.
 
## Synchronization

Shared variables, global or static, are prone of being out of sync if multiple threads are perfoming concurrent operations involving updating the variable. Primarily because breaking down an instruction into loads and saves can lead to a variable being saved in some thread after it was loaded and saved by another thread. This leads to the concept of a *progress graph*: an n-dimensional graph where each dimension represents a thread, and each discrete point on some dimension the current progress of an instruction. A vector is the progress of all dimensions at a certain point in time. This leads to discrete areas of *critical sections* that consitute an unsafe region. Tragectories skirting an unsafe region are safe. The idea is to make a global variable's access mutually exclusive to a particular thread until the *critical section* passes.

### Semaphores

A global variable of value 0 or 1. Used to regulate the usage of other common variables between concurrent threads. There need be a P routine that either suspends the operation if the semaphore is 0, or decrements the semaphor and returns immediately. A suspended thread is *holding* the semaphore value of 0 at P, and it restarted by a V operation that increments the semaphore and signals that the semaphore value has been incremented to any *one* of the holding threads. 

~~~c

int sem_init(sem_t *sem, 0 , unsigned int value); /* sets the semaphore to value
                                                   * w/a mysterious middle value
                                                   * always set to 0
                                                   */
int sem_wait(sem_t *s); //P
int sem_pot(sem_t *s);  //V
~~~

Adding a P and V operations before and after unsafe regions insures that only one thread is performing a set of (now protected) instructions at any given point in time.

### Scheduling Shared Resources

Producer-Consumer:

A problem involving a producer, a consumer and a shared buffer between them. The basic idea is to make use of counter semaphores and mutexes to enforce some form of schedueling between the consumer and the producer. It becomes a way of communication between threads which is a subtle difference from before; threads are locked until a semaphore is changed by another thread which acts as a continue signal.

Readers-Writers:

Many concurrent readers, exclusive writers, both unbounded. First, a preference must be given to writers or readers based on who should wait until whom is done. If a reader arrives after a writer but it is given preference, then the writer must wait until the reader is finished, and the reverse is true if the preferences were opposite. The issue ofcourse, is that a stream of either can block the other indefinetly.

## Parellelisms

A parallel program is a concurrent program running on mutliple processors. Synchronization routines P and V are *EXTREMELY* expensive and should be avoided if possible or amortized over useful computation my guy. Properly synchronized threads run in parallel, with the mindset of coding one thread per core. Higher threads leads to more overhead introduced by context switching. There are three measures of performance here:

1.  Running time. S<sub>p</sub> = T<sub>1</sub>/T<sub>p</sub>. Measures the absolute or relative running times depending on whether or not a comparison is being made to a sequential program or a parallel one running with a single thread. Also referred to as *strong scaling*.
2.  Efficiency: E<sub>p</sub> = S<sub>p</sub>/p. Measures the amount of overhead introduced by parallelism.

## Concurrency Issues

1.  Static variables are not protected: library routines especially that store static variables and return pointers. A *lock-and-copy* technique is used in defining a wrapper function where the pointer is dereferenced and copied elsewhere after and before locking and unlocking the mutex.
2.  Functions calling thread-unsafe functions.
3.  Functions that do not protect shared variables.
4.  Functions that keep state between invocations.

A simple solution to most of these issues is to use the linux standard reentrant functions of otherwise normal functions (with the suffix _r added), such as gethostbyaddr_r.
