// Don't fear the void cast

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>


using namespace std;


void copy_int(int val, void *buff, int maxbytes){
    if (maxbytes >= (int) sizeof(val)){
        memcpy(buff, (void *) &val, sizeof(val));
    }
}

int main(){

  int maxbytes = sizeof(int) * 10; // space for 10 integers or 80 bytes
  void* buf = malloc(maxbytes); // returns a pointer to a memory location of size 10 integers
  int val;

  val = 0x12345678; //assigns random value to the int we wish to copy
  copy_int(val, buf, maxbytes); // passes the address of buffer, the value to be copied and the size of buffer set by malloc
  assert(*(int*)buf == val); // generic pointers have to be cast to a specific type before dereferencing

  free(buf);

}