#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
using namespace std;
int is_little_indian(){
    int test_byte = 0xff;
    byte_pointer start = (byte_pointer) &test_byte;
    if (start[0] == 0xff){
        return 0;

    } else {
        return 1;
    }

}
int main (){
int whatis = is_little_indian();
cout << (( whatis == 0) ? "This machine is Little Endian" : "This machine is Big Endian");


}