#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>

int mul3div6(int x){

    int bit_check = x >> ((sizeof(int) << 3) -1);
    x= (x << 1) + x;
    int bias = bit_check *((1 << 4) - 1);
    int div_x = (x + bias) >> 4;
}
int main (){

}
