#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;

void test_int(string &start, size_t len){
    int i;
    int e=0;
    for (i=0; i < len; i++)
    {
    if(start[i] == 0x2e || start[i] <0x30 || start[i] > 0x39 ) throw e;
    
    }
   
   
}


int main()

{
    string anyNumber;
    cout << "Input any number\n";
    cin >> anyNumber;
    try{
        test_int(anyNumber, anyNumber.length());


    } catch(int notInt)
    {
        cout << "Not an integer, exiting\n";
        exit(0);
    }

    test_show_bytes(stoi(anyNumber));
  

}

