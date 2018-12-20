#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum)
{
	unsigned temp = word >> ((bytenum) << 3) & 0xFF;
	int n = (sizeof(int)-1) >> 3;
	temp = temp << n;
	return (int) temp >> n;
}

int main()
{
	packed_t x = 0xABCDEF;
        printf("Byte 0 of 0xABCDEF is %#x:\n", xbyte(x,0));


}