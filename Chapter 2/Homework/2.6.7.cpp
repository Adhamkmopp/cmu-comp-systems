#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;


int main (){

unsigned test = ~0x0;
test <<= 31;
bool is32 = (test == (1<<31));
printf("This machine represents ints as 32-bits: %s", is32 ? "true" : "false");}
