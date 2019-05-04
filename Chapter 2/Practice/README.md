# Chapter 2: Representation & Manipulation
## The BigLittle-Endians

The following code casts an initial integer value as a float and a pointer, and outputs the byte-level representation of all three by casting them as byte-size pointers (as char). On an intel machine (like mine), the output is big-endian, with the most significant digit stored in the lowest memory adress onwards:

```c++
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

```
What's interesting is the missed point of introducing pointer arithmetic. In the show bytes function, the byte pointer can simply be treated as a counter in and of itself by incrementing its value by 1, dereferncing and printing until the max length is reached, as shown here:

```c++
void show_bytes(byte_pointer start, size_t len){
    int i;
    for (i=0; i < len; i++)
    printf(" %.2x", *start+i);
    printf("\n");
}
```
Alternatively, if the pointer is of a different size, an increment of 1 will equate a jump in address by some number of bytes corresponding to the pointer type (by 4 bytes in the case of int for example, 8 for floats).

## Integer Representation

The code below converts the signed and unsiged short representations (2 bytes) into their int equivalents (4 bytes) and prints their byte representations

```c++

#include <stdio.h>
#include "byte.h"


int main(){

short sx=-12345;
unsigned short usx = sx;
int x = sx;
unsigned ux=usx;

printf("sx = %d:\t", sx);
show_bytes((byte_pointer) &sx, sizeof(short));
printf("usx = %u:\t", usx);
show_bytes((byte_pointer) &usx, sizeof(unsigned short));
printf("x = %d:\t", x);
show_bytes((byte_pointer) &x, sizeof(int));
printf("x = %u:\t", ux);
show_bytes((byte_pointer) &ux, sizeof(unsigned));


};
```

With output

sx = -12345:	 c7 cf
usx = 53191:	 c7 cf
x = -12345:	 c7 cf ff ff
x = 53191:	 c7 cf 00 00

The output shows that signed expansion is done through sign extension (oxFF) and unsiged expansion is done via zero extension. The main takeway is that bit level representations may be inconsistent between integer expansions, specifically for signed integers.

## Division

Easy division of two's complement without comparison operators, modulus or actual straight division. Also includes partly modified code of show_bytes to detect decimals if present and exit the program through a basic try-throw-catch pattern passed to the main function.



```c++
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

typedef unsigned char *byte_pointer;

int div6 (int x){
int bias = (x >> 31) & 0xf;
return ( x +bias )>> 4;
}


void show_bytes(string &start, size_t len){
    int i;
    int e=0;
    for (i=0; i < len; i++)
    {
    if(start[i] == 0x2e) throw e;
    
    }
   
   
}


int main(){
    string x;
    cout << "Input an integer, positive or negative \n";
    cin >>x;
    try{
        show_bytes(x, x.length());


    } catch(int notInt)
    {
        cout << "Not an integer, exiting\n";
        exit(0);
    }
    cout << div6(stoi(x));
}


```

## Floating Points

### IEEE. What Is It ?:

Structure wise, a binary word is split into three components; a sign bit s,  k exponent bits and an n bit fraction field. It gets slightly more complicated with the introduction of more terms in the following formula needed for the actual calculation of the floating point decimal:

V = (-1)<sup>s</sup> * M * (2<sup>E</sup>)

But what is M and E? They are derived from f and e in different ways depending on the context:

M is the  ranges from 0 to 1 - e  or 1 and 2 - e
and
E is the exponent, which weighs it by powers of 2

The logic applied here is to compute basic fractional binary biners for the exp and frac fields, and apply some stupid transformation on them depending on the three situations deemed by Kahan and his goons at the IEEE.

### Three Situations:

I. Normalized: The high lords of the IEEE have determined that when the exponent is neither all 0's or all 1's, E ranges in value from -126 to 127 for single precision; E = e - Bias (2<sup>k-1</sup> - 1) and  M is 1 + f.  The lowest value is never 0 and the maximum value is astronomical for single and double precision formats.

II. Denormalized: Occurs when the exponent is all zero. In that case, E is 1 - Bias and M = f. These represent values that are very close to 0 but not quite.

III. Special: All 0's is zero, and when the exponent is all 1's, it's infinity.

I believe the benefit of all of this is to allow a somewhat uniform distribution of numbers that carries over to whole integers up to very high values while allowing for the approximation fractional decimal numbers (huge guess here). The system is also set up such that in unsigned form, it appears that increasing values is concurrent with increasing floating point values as well, allowing for integer sorting routines to be applied directly unto floating numbers (100% fact). 

Finally, there are other bullshit terms like mantissa that should be avoided like the plague out of respect for human decency.

### Rounding:

Round-to-even (default): Closest mark. Halfway values are roudned such that the result has an even number in the least significant digit. The logic of which means that statistically, a set of points will have an average that is neither over or under estimated than if we had always rounded up or down.

Round-to-zero (float to int): Always towards zero which only becomes apparent when one considers the sign.

### Registers and Such:

There are 16 ymm or xmm registers holding 32/16 bytes (256/128 bits). Beyond that should be looked up on a case-by-case basis. The main gist of all of this is that even though floating point numbers are ugly and cumbersome, they can be used to exploit parallelism by performing the same opereation on a number of data points in parallel represented as packed data (SIMD). Details forthcoming in chapter 5.



