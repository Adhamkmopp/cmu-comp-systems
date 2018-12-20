#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

int leftmost_one (unsigned x){
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return (x - (x >> 1));

}

int main (){

int anyNum;
cout << "Input any number including zero" << endl;
cin >> anyNum;
int bit_test = leftmost_one(anyNum);
printf("The bitmask is: %#x", bit_test);
}
