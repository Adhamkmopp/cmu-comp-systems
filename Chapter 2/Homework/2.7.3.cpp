#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>


using namespace std;

int saturating_add(int x, int y){
    int sign_shift = (sizeof(int) << 3) -1;
    int sum = x + y;
    int sgn_x = x >> sign_shift;
    int sgn_y = y >> sign_shift;
    int sgn_s = sum >> sign_shift;
    int positive_bit = (sgn_s && !sgn_y && !sgn_x) << sign_shift;
    positive_bit >>= sign_shift;
    int negative_bit = (!sgn_s && sgn_y && sgn_x) << sign_shift;
    negative_bit >>= sign_shift;
    int min_int = (!sgn_s && sgn_y && sgn_x) << (sign_shift) ;
    int max_int = ~ ((sgn_s && !sgn_y && !sgn_x) << (sign_shift));


    return ((positive_bit & max_int) | (negative_bit & min_int) | (sum & ~ (positive_bit | negative_bit)));
}

int main (){
printf("Positive saturation for min int -1 is: %d \n", saturating_add(INT_MIN	, -1));
printf("Negative saturation for max int + max int is: %d \n", saturating_add(INT_MAX, 1));
printf("Normal addition for 2+2 is: %d \n", saturating_add(2, 2));

}