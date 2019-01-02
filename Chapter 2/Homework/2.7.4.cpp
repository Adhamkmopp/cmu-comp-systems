#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>


using namespace std;

int tsub_ok (int x, int y){
    int sign_shift = (sizeof(int) << 3) -1;
    int sum = x - y;
    int sgn_x = x >> sign_shift;
    int sgn_y = y >> sign_shift;
    int sgn_s = sum >> sign_shift;
    int neg = sgn_s && !sgn_x && sgn_y;
    int pos = !sgn_s && sgn_x && !sgn_y;

    return (neg || pos);

}

int main (){

    printf("Negative overflow occurs when their signs differ such that x is negative and the result exceeds TMax: %d \n", 
    tsub_ok(INT_MAX	, INT_MIN ));
    printf("Positive overflow occurs when their signs differ such that x is positive and the result exceeds TMin: %d \n", 
    tsub_ok(INT_MIN	, INT_MAX ));
    printf("Normal operations when the operands are of the same sign regardless of value: %d \n", 
    tsub_ok(0	, INT_MAX ));

}