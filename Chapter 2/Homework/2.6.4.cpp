#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

int any_odd_one(unsigned x){
return ( (x | 0xAAAAAAAA) == 0xAAAAAAAA);
}

int main (){

int anyNum;
cout << "Input any number including zero" << endl;
cin >> anyNum;
int bit_test = any_odd_one(anyNum);
cout << "Number includes an odd 1 bit somewhere:" << bit_test << endl;
}