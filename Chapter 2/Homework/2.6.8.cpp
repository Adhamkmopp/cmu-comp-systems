#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

int lower_one_mask(int n)
{
    int hex_rep = 0x80000000;

    int shift = 31-n;
    hex_rep = (hex_rep >> shift);
    return ( ~ hex_rep);
}
int main (){
int anyNum;
cout << "Input any number including zero to be represented as a lower bit mask" << endl;
cin >> anyNum;
int bit_test = lower_one_mask(anyNum);
printf("The bitmask is: %#x", bit_test);

}
