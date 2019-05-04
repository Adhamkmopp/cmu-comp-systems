#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(){

int hello  = open("hello.txt", O_RDWR, 0 );

struct stat stat;
fstat(hello, &stat);
off_t size = stat.st_size;
char *buf = mmap(NULL, size,PROT_WRITE , MAP_PRIVATE ,hello ,0 );

*buf = 'J';

write(1, buf, size);
munmap(buf, size);
close(hello);

    
}