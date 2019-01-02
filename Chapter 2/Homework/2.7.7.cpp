#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>

void cheap_multiplier (int x){
    int k_1 = (x << 4) + 1;
    int k_2 = x - (x << 3);
    int k_3 = (x << 6) - (x << 2);
    int k_4 = (x << 4) - (x << 7);
    
}

int main (){

}