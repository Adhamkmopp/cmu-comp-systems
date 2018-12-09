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

