#include <stdio.h>
#include <iostream>
#include <string.h>
#include "byte.h"
#include <assert.h>
#include <limits.h>


void * calloc(size_t nmeb, size_t size){

    size_t bytes= size * nmeb;
    void * ptr = malloc(bytes);
    memset (ptr, 0, size*nmeb);
}
int main (){

}

