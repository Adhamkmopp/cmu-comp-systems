#include <stdio.h>
#include <iostream>
#include <string.h>
#define OP +

typedef long data_t;
static const int vbytes = 32;
static const int vsize = vbytes/sizeof(data_t); // number of elements
static const int ident = 1;

typedef data_t vec_t __attribute__ ((vector_size(vbytes))); // ESSENTIAL for any SIMD instructions


typedef struct{
    long len;
    data_t *data; // pointer to the beginning
} vec_rec, *vec_ptr;


vec_ptr new_vec(long len){
/* takes in a length and outputs a pointer to the new vector */
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data=NULL;
    if (!result)
        return NULL;    /* Could not allocate storage */
    result ->len=len;
    if(len>0)
        data = (data_t*)calloc(len, sizeof(data_t)); // initiates an array of len elements of size data_t each
        if(!data){
            free((void*) result);
            return NULL;    /* Could not allocate storage */
        }
        result ->data = data;
        return result; 
}

int get_vec_element (vec_ptr v, long index, data_t *dest){
    if (index < 0 || index >= v->len)
        return 0;
    *dest = v ->data[index];
    return 1;
}

long vec_length(vec_ptr v){
    return v -> len;
}

data_t* get_vec_start(vec_ptr v){
    return v ->data;
}




/* Compute inner product of SSE vector */
data_t innerv(vec_t av, vec_t bv) {
long int i;
vec_t pv = av * bv;
data_t result = 0;
for (i = 0; i < vsize; i++)
    result += pv[i];
return result;
}


void simd_v1_combine(vec_ptr v, data_t *dest){

long i;
vec_t accum;
vec_t accum0;
vec_t accum1;
vec_t accum2;
vec_t accum3;

data_t *data = get_vec_start(v);
int cnt = vec_length(v);
data_t result = ident;

/* Initialize all accum entries to IDENT */

for (i = 0; i < vsize; i++)
{
    accum[i] = ident;
    accum0[i] = ident;
    accum1[i] = ident;
    accum2[i] = ident;
    accum3[i] = ident;
}

/* Single step until have memory alignment */
while ((((size_t) data) % vbytes) != 0 && cnt) {
    result = result OP (*data)++;
    cnt--;
}
    
/* Step through data with VSIZE-way parallelism */
while (cnt >= 4*vsize) {
    vec_t chunk0 = *((vec_t *) data);
    vec_t chunk1 = *((vec_t *) data + vsize);
    vec_t chunk2 = *((vec_t *) data +(2*vsize));
    vec_t chunk3 = *((vec_t *) data +(3*vsize));
    accum0 = accum0 OP chunk0;
    accum1 = accum1 OP chunk1;
    accum2 = accum2 OP chunk2;
    accum3 = accum3 OP chunk3;
    data += 4*vsize;
    cnt -= 4*vsize;
    accum = accum OP ((chunk0 OP chunk1) OP (chunk2 OP chunk3)); // 4 x 1a
}
accum0 = (accum0 OP accum1) OP (accum2 OP accum3);

/* Single-step through remaining elements */
while (cnt) {
    result = result OP *data++;
    cnt--;
}

/* Combine elements of accumulator vector */
for (i = 0; i < vsize; i++)
    result = result OP accum0[i];
/* Store result */
 *dest = result;
}



int main (){

    
    /* Crate a new vector of specified length */



}

