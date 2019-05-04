
~~~c
char * strstr ( const char * str1, const char * str2 );
~~~

Returns a pointer of the first occurance of str2, or NULL if none exist. Longer version of strchr.



~~~c
char * fgets ( char * str, int num, FILE * stream );
~~~

Reads characters from a stream until num or a newline is encountered. Works on FILE types, not applicable to system level file descriptors.

~~~c
char * strcat ( char * destination, const char * source );
~~~

Makes a copy of source to destination up to the null terminating character, and returns the destination as well.

~~~c
while(1){
    /* perpetual work */
}

while(!condition){
    /* work that will eventually change the status of the conditoin */
}

~~~
Useful for keeping an operation going for eternity, occasionally checking for a condition to move on.

~~~c
argv[argc++] = buf;
	*delim = '\0';
~~~

Useful for splitting up an array of char's into words, like so char 'arr[] = "abc\0def\0ghi\0";'.

~~~c
int ferror ( FILE * stream );
~~~

Checks if the error indicator associated with stream is set, returning a value different from zero if it is.

~~~c
int fflush ( FILE * stream );
~~~

"If the given stream was open for writing (or if it was open for updating and the last i/o operation was an output operation) any unwritten data in its output buffer is written to the file."

~~~c
void (*signal(int sig, void (*func)(int)))(int);
~~~

The functoin takes and int sig and a pointer to a function that takes an int and returns nothing, and returns a pointer to another function that returns nothing and takes an int.

*NOTES*

1.  Sockets are index file desciptors, not streams
2.  fstat and stat cannot be used on index file descriptors
3.	extern specifically tells the compiler that the variable is defined elsewhere
4.	R_386_COPY is used in dynamic linking for defining extern variables that reference global variables in the .so. The relocation entry then changes the address of the variable to the address defined in the executable, not in the .so.
5.	function pointers: void (*addvec) (int *, int *, int *, int) is a pointer valled addvec to a function that returns nothing and takes in 3 pointers and an int as parameters.
6.	__i686.get_pc_thunk.bx: mov(%esp), %ebx: loads the current instruction address into ebx
7.  The entire stack is almost always on the CPU cache
8.  (Random fact) C++ only allows *widening* implicit casts, not shortening, and void pointer casts are *disallowed*.
9.   volatile: useful for shared global variables; volatile variables are never stored in registers, always read and written from and to memory.
10. atomic: uninterruptible variables, in the sense that single read/write instructions cannot be interrupted by a signal. Other operations that involve addition for example, can still be interrupted.
11. In C, func(&ptr) is equivalent to func(char **ptr); In other words, a pointer to a pointer is created and passed through the function. It is not, as in C++, a pass by reference which is illegal in C. The getaddrinfo *result* is exactly that.
12. There is no "EOF" character at all, it is a condition, and different functions or processes define what that condition is. It may be as simple as a zero returned on function exit, or when the current file position exceeds the file length in disk files. Closed conections automatically sends an "EOF".
13. (void *) -1 == (size_t) -1: It is the maximum address available on a machine's own address space.
14. In thread synchronization, the semaphore functions P and V are indivisible (they run without interruption/sole CPU instruction).
15. Thread routines return a generic null pointer, but to whom? *To the (void \*\*) return pointer argument given to the join function*.
16. Pass unique thread IDs and make use of them whenever possible.
17. Use reentrant functions (suffix:_r) whenever possible.
18. Never pass arguments on the local stack to peer threads. If the value is to be updated like in a for loop, then a race condition is introduced between derferencing the value in the thread routine, and updating the value for the next peer thread. Use Malloc instead.