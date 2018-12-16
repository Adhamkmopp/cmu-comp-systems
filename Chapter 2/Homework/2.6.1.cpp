
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"


using namespace std;
int main (){
int anyNum;
cout << "Input any number including zero" << endl;
cin >> anyNum;
int bit_test = anyNum ^ 0x0;
bool bits = !!bit_test;
cout << "Number includes a 1 somewhere:" << bits << endl;

bit_test = anyNum ^ ~0x0;
bits = !!bit_test;
cout << "Number includes a 0 somewhere:" <<  bits << endl;

int shift_val = (sizeof(int)-1)<<3;
char msb = (char) (anyNum >> shift_val);
char lsb = (char) anyNum;
char bit_test_new = lsb ^ 0x00;
bits = !!bit_test_new;

cout << "Number includes a 1 in the least signfificant bit:" << bits<< endl;

bit_test_new = msb ^ 0x00;
bits = !!bit_test_new;
cout<< msb;
cout << "Number includes a 1 in the most signfificant bit:" << bits<< endl;

}
