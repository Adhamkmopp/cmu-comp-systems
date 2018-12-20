#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

int fit_bits(int x, int n){

int test = (x >> n-1);

return (test==-1 || test == 0);
}

int main(){

    printf("The number 2 can be represented in 2 bits two's complement form: %s \n", fit_bits(2,2) ? "true" : "false");
    printf("The number 2 can be represented in -2 bits two's complement form: %s \n", fit_bits(-2,2) ? "true" : "false");



}