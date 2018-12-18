#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

unsigned srl (unsigned x, int k){
    unsigned xsra = (int) x >> k;
    int size = sizeof(int) * 8;
    return (xsra | ~(0x1 << (size -1)) >> k);
}

signed sra(int x, int k){
    int xsrl = (unsigned) x>>k;
    int size = sizeof(int) * 8;
    return (xsrl | ~(0x1 << (size -1)) >> k);
}


int main (){


}
