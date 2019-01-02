#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>

int bit_pattern(int j, int k){

   int pattern_a = -1;
   pattern_a <<= k;

   int x = -1;
   int y = -1;
   x <<= j;
   y <<= (k+j);

   int pattern_b = ~ ( x | ~y);

}
int main (){

}
