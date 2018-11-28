#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"

using namespace std;


int main()

{
    int anyNumber;
    const char *m = "abcdefg";
    

    cout << "Input any number\n";
    cin >> anyNumber;
    test_show_bytes(anyNumber);

    show_bytes((byte_pointer) m, strlen(m));

}

