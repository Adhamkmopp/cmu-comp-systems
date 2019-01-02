#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>


using namespace std;

int signed_high_prod(int x, int y) 
{
    x = (int64_t) x;
    y = (int64_t) y;
  int64_t high_signed = x * y;
  return (uint64_t) high_signed >> 32;
}


unsigned unsigned_high_prod(unsigned x, unsigned y){
    int bit_shift = (sizeof(int) << 3) - 1;
    int sgn_x = x >> bit_shift;
    int sgn_y = y >> bit_shift;
    int high_unsigned = signed_high_prod (x,y) + (y * sgn_x ) + (x * sgn_y );
     return (high_unsigned);

}


int main (){
    uint unsigned_max = 0xFFFFFFFF;
    int signed_max = 0xFFFFFFFF;
    printf("Unsigned product of 0xFFFFFFFF and 0xFFFFFFFF is: %#x%x \n", 
    unsigned_high_prod(unsigned_max,unsigned_max), (unsigned_max * unsigned_max));
}
