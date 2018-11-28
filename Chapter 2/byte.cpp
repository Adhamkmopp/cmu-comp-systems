#include <stdio.h>
#include "byte.h"

using namespace std;

void show_bytes(byte_pointer start, size_t len){
    int i;
    for (i=0; i < len; i++)
    printf(" %.2x", start[i]);
    printf("\n");
}

void show_int(int x){
/*
same as declaring a byte_pointer z= &x and passing z directly.
z as a pointer adress is passed a copy however.
/**/
    show_bytes((byte_pointer) &x, sizeof(int));
    

}

void show_float(float x){
    show_bytes((byte_pointer) &x, sizeof(float));
}


void show_pointer(void *x){
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void test_show_bytes(int val){
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int(val);
    show_float(fval);
    show_pointer(pval);
}