# Chapter 2: Little/Big-endian

The following code casts an initial integer value as a float and a pointer, and outputs the byte-level representation of all three by casting them as byte-size pointers (as char). On an intel machine (like mine), the output is big-endian, with the most significant digit stored in the lowest memory adress onwards:

```c++
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

typedef unsigned char *byte_pointer;

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

int main()

{
    int anyNumber;
    const char *m = "abcdefg";
    

    cout << "Input any number\n";
    cin >> anyNumber;
    test_show_bytes(anyNumber);

    show_bytes((byte_pointer) m, strlen(m));

}

```

