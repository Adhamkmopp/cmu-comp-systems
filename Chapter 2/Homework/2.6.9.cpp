#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

unsigned rotate_left(unsigned x, int n){
int upper_mask = 0x80000000;
upper_mask >>= (n-1);
unsigned curved_x;
curved_x = upper_mask & x;
curved_x >>= (32-n);

return ((x << n) | curved_x);

}

int main(){

    unsigned test = 0x12345678;
    printf("Left rotated test by 2 positions is : %#x \n", rotate_left(test,2));
    printf("Left rotated test by 20 positions is : %#x \n", rotate_left(test,20));
    printf("Left rotated test by 4 positions is : %#x \n", rotate_left(test,4));

    
}