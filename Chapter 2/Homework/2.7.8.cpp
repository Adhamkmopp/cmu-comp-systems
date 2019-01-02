#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>

int divide_power2(int x, int k){
    int bit_check = x >> ((sizeof(int) << 3) -1);
    int bias = bit_check *((1 << k) - 1);
    int div_x = (x + bias) >> k;
    return (div_x);
}
int main (){

}