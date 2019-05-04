#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define ALIGNMENT 4
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT - 1))

int main (){

    printf("%d, %d , %d\n", ALIGN(1), ALIGN(7) , ALIGN(9));
}