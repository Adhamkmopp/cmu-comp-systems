# Chapter 10: System-Level I/O

## Unix I/O: *Systems Calls*

All devices are modeled as files: ordered, linear array of bytes with consitent methods associated with that abstraction:

1.  Opening files: a file descriptor is given to the application while the kernel keeps track of the file itself.
1.  Changing postion: a file positon is maintained by the kernel and may be changed by explicitly performing a seek operation.
1.  Reading and Writing: reading is copying a file to memory, writing is copying from memory to file. This involved an intermediate memory buffer. An EOF may trigger in reading (no explicit symbol for EOF).
1.  Closing: a file is closed when the descriptor is restored and memory is freed.

## Files

1.  Regular file: like binary and text files. A newline character in text files is 0x0a in ASCII (line feed character).
1.  Directory: a file too, made up of an array of links to filenames of other files which may be directories or regular files. A dot is link to the directory itself, a dot-dot is a link to the parent.

## Grand Opening

~~~c
int open (char *filename, int flags, mode_t mode);
/* returns the file descriptor associated with the open file or -1 on error */
~~~

The flags include read and write privelages alongwith more specific instructions such as O_APPEND which sets the file position to the end of the file to allow appending instead of overwriting. The mode sets permission modes in the most obnoxious way possible.

## Reading and Writing

~~~c
ssize_t read(int fd, void *buf, size_t n);
ssize_t write(int fd, const void *buf, size_t n);
/* returns number of bytes transferred, 0 on EOF and -1 on error */
~~~

The mystery of ssize_t: it means signed long instead of unsigned long which is regular size_t. It halves the limit to be able to return -1. It also sets errno since it is a system call. Bizzaro. I also *think* it internally shifts the file position by the number of bytes read but I'm not so sure.

## Robuts Reading and Writing

Allows for different kinds of functionality:

1.  Unbuffered: directly to and from memory and file with no *application-level* buffer whatever that means.
1.  Buffered: allows caching of contents in an *application-level* buffer like printf.


~~~c
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
/* returns number of bytes transferred, 0 on EOF and -1 on error */
~~~

To understand the code properly, one must keep in mind that read and write freak out when interrupted by a signal; both functions return -1 and set errno to EINTR. The code restarts infinitely as the case may be, until the function returns 0 (reaches EOF in read()).


For buffered functions, a different kind of buffer in the form of a struct is introduced:

~~~c
void rio_readinitb(rio_t *rp, int fd){
    rp -> rio_fd = fd;
    rp -> rio_cnt = 0;
    rp -> rio_bufptr = rp -> rio_buf;
};
/* initializes the so-called application level buffer *rp  */

ssize_t rio_readlineb (rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb (rio_t *rp, void *usrbuf, size_t n);
/* both are buffered versions with different functionalities 
   returns -1 on error, 0 on EOF or number of bytes read*/

typedef struct{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr; /* rio_buf pointer
    char rio_buf[RIO_BUFSIZE]
} rio_t
~~~


The readlineb function terminates each newline with a NULL character, chopping any line that exceeds maxlen. Both functions run atop the main read function:

~~~c
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);
~~~

The rio_read is a wrapper function of read(). It takes an rp struct (after initialization), a memory buffer to fill and total number of bytes to read. It fills the rp's *application-level* buffer to its limit and updates rp -> rio_cnt (bytes read) from the return status. It also restarts the function incase of a signal interruption. Afterwards, it uses the memcpy functoin to copy over the contents of rp->rio_buf to usrbuf and updates the position of rio -> rio_bufptr +=cnt which may or may not fill the entire length of the buffer. The buffer pointer is reset incase the buffer is full. The other functions readlineb and readnb call rio_read repeatedly to perform whatever functions they were designed to. Bottom line is, the kernel keeps track of where it is in the file internally and does not care where the user's buffers are; it will either overwrite or keep writing depending on the pointer location fed into the read() function itself.

## Metadata

~~~c
int stat (const char *filename, struct stat *buf);
int fstat (int fd, stuct stat *buf);
/* 0 if ok, -1 on error
~~~

The stat struct is filled with the metadata of the file including a concatenate permission bits and type member called st_mode, which can be deciphered by a macro (S_ISREG(m), S_ISDIR(m), S_ISSOCK(m)).

## Reading Directories

Each directory item is a *dirent* struct containing two members; one with a weird type encoding the file location -"inode number"- and the other a char array of the filename [256 bytes]. The opendir function opens a *stream* which according to the book is an abstraction of an ordered list of items. It also alludes to the fact of it being similar to a stream, that another function is needed to sequentially return pointers to the items in the stream in the form of the structs described above.


## Sharing Files

There are three tables the kernel maintains for keeping track of all open files, some are visible to all processes, some are not:

1.  Descriptor table: exclusive to each process, it contains a record entry for each open file, the fd of all open files and a pointer to its *File Table*
1.  File table: shared by all processes and unique for each file, the table contains an entry with the current file positon (for which process? all of them? the book state "for our purpose" in paranthesis whatever that means), the number of processes pointing to it and a pointer to a v-node table.
1.  V-node table: shared by all processes again, this is a table with the entries of the dirent struct, it seems.

It is noteworthy to keep in mind that a child process inherets the parent's open file descriptors; a duplicate of the descriptor table. The consequence of that is that a child reading the file will shift the reading position by some distance for the parent as well. Modifying a file somewhere affects its position elsewhere.



*I/O Redirection*: For writing to files, changing the file descriptor with the function dup2 (strange name), is equivalent to redirection.

## Standard I/O

C provies higher level functions such as fopen, fgets and scanf instead of the Unix I/O functions. These are built atop a stream which is a pointer to a FILE struct, which in and of itself is an abstraction of a file descriptor and a *stream buffer*. It operates exactly the same way as RIO and its purpose is to reduce system calls by buffering as much as possible.

fseek:
fsetpos:
rewind:
