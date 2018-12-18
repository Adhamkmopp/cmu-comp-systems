
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

int int_shifts_are_arithmetic (){
    int testNum= -1;
    int shift_size = ((sizeof(int) -1) << 3) + 7;

    int newNum = testNum >> shift_size;
    cout << newNum << endl;
    bool res;
    res = (newNum ^ testNum);
    return res;

}

int main (){

int endresults = int_shifts_are_arithmetic();
    cout << "This machine performs uses right shifts that are: "<<endl;
    cout << "Arithmetic: " << endresults << endl;
    cout << "Logical: " << !endresults <<endl;


}
