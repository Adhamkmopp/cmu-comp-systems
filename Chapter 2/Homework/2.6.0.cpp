
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"


using namespace std;
int replace_byte(unsigned &old_hex, int loc, unsigned rep_hex){
    byte_pointer new_hex = (byte_pointer) &old_hex;
    new_hex[loc] = rep_hex;
}
int main (){
    unsigned x = 0x12345678;
cout << "Previously, the old hex was "; 
show_bytes((byte_pointer) &x, sizeof(unsigned)); 
cout << "The new hex is "; replace_byte(x, 2, 0xAB);
show_bytes((byte_pointer) &x, sizeof(unsigned)); 

}