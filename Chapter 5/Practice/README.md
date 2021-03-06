# Chapter 5: Optimizing Program Performance

"Determining which combinations of transformations to apply is indeed part of the "black art" of writing fast code."

## General Optimization:

I.	Code motion: repetitive operations whose value is immutable throughout execution may instead be subtituted with a stored variable on the stack or in register after a single operation.
II.	Strength reduction: Multiplication to shifting and addition, if possible. Compilers do this efficiently anyway, but it would not hurt to account for it explicitly.
III.	Common subexpressions: Reuse partial subexpressions in more complicated ones. It can reduce the number of operations which may save cost greatly (esecially in the case of multiplication instructions).


## Optimization blockers:

Specifically for compilers, some code aspects block certain optimizations. These include:

I.  Memory aliases: if a compiler cannot determine whether or not pointers will be aliases/refer to the same location. *Use intermediate values for storage*.
II. Function calls: repeated function calls are costly where a reduced number of calls is not (bookeeping: parameters, local variables, rbp and rsi are saved/set each time). A function call may need repeated calls however, blocking inline substituion. *Use inline substitution whenever possible*.

## Critical paths & Overhead: 

Critical paths are chains of dependencies that slow down a program through repeated execution by blocking parallisms. This is only meaningful in the context of clock cycles spent per instruction, which in turn, is made clear after learning about the hardware of the cpu and what clocked registers and combinatorial circuits mean, and how they're updated. "Wasteful" time spent is a clock cycle wasted, which generally means (aside from optimizing basic arithmetic instructions) that a register was updated when it did not have to, or data was involved in a memory operation which otherwise could have been avoided. *Improving performance == reducing the number of operations per cycle*.

## CPU Design

Modern CPUs are superscalar in the sense that multiple instructions may be executed on every clock cycle and out of order. There are two main organizational parts; the instruction control unit (ICU) and the execution unit (EU). Detailed inner workings are complex, but the main gist of it is that instructions are fetched, and decoded (broken down into microinstructions) and sent to the EU where multiple instructions running on multiple functional units is performed each cycle. Addtionally, the entire process is speculative with tight controls to determine branching success or failure. An instruction is not fully retired until all associated branches leading up to it have been confirmed as correctly predicted, upon which the register file is updated.

## Latency Bounds: Reducing Operations

There is latency, and there is issue. Latency is the time measured in clock cycles for one operation to complete from start to finish, while issue is the minimum number of cycles between two independent operations. Most operations have an issue of 1 due to pipelining (segmenting an operation into smaller tasks). On top of that, there may be multiple copies of each functinal operation unit such as addition that allow parallelism. This introduces throughput, which is the same as issue but after parallelism is taken into account; it is the reciprocal of the issue time multiplied by the number of functional units.

The main goal of approaching latency bounds is through reducing overhead, and eliminating operations involved in loops that may form unnecessary dependancies (pointer arithmetic). It can be acheived by a number of techniques performed in sequence here.

Below is the code for allocating a vector and two more functions for retrieving elements and lengths which will be used in the subsequent 5 refinements over a code that performs some arithmetic operation on all elements of the vector.

```c++

typedef long data_t;

typedef struct{
    long len;
    data_t *data;
    } vec_rec, *vec_ptr;


vec_ptr new_vec(long len){
    /* Allocate header structure */
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data=NULL;
    if (!result)
        return NULL;    /* Could not allocate storage */
    result ->len=len;
    if(len>0)
        data = (data_t*)calloc(len, sizeof(data_t));
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

data_t *get_vec_start(vec_ptr){
    return v -> data;
 }
```

### Pure Form

The first iteration of a sigma operation is slow, because it calls vec_length repeatedly each iteration of the loop with all that entails (possibly storing rbp and updating rsp).

```c++

void combine1 (vec_ptr v, data_t *dest){
    long i;
    *dest = 0;
    for (i = 0; i <vec_length(v); i++){
        data_t val;
        get_vec_element (v, i , &val);
        *dest = *dest + val;
    }
}
```


### Reducing Function Calls

The second iteration is much faster by comparison as it eliminates repeated function calls to vec_length.

```c++

void combine2 (vec_ptr v, data_t *dest){
    long i;
    long length = vec_length(v);
    *dest = 0;
    for (i = 0; i <length; i++){
        data_t val;
        get_vec_element (v, i , &val);
        *dest = *dest + val;
    }
}
```

### Reducing Function Calls (Again)

The third iteration does away with function calls entirely by setting a pointer to the start of the vector and using pointer arithmetic to fetch the next element (a load operation running in parallel to the multiplication/addition operation every clock cycle).

```c++
data_t *get_vec_start(vec_ptr v){
    return v ->data;
}

void combine3 (vec_ptr v, data_t *dest){
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    
    *dest = 0;
    for (i = 0; i <length; i++){
        *dest = *dest + data[i];
    }
}
```

This should have theoretically led to much higher performance improvements, but it did not. There are other bottlenecks that are not yet apparent in code.

### Eliminating Memory References

The fourth iteration eliminates memory references by avoiding repeated calls to dereference *dest and storing the data in a seperate variable instead (to be stored in regsiter xmm0).
```c++
data_t *get_vec_start(vec_ptr v){
    return v ->data;
}

void combine4 (vec_ptr v, data_t *dest){
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = 0;
    
    
    for (i = 0; i <length; i++){
        acc = acc + data[i];
    }
    *dest = acc;
}


data_t *get_vec_start(vec_ptr v){
    return v ->data;
}
```

## Loop Unrolling

Loop unrolling simply reduces loop overhead and unecessary operations such as branch prediction and condition checks by performing multiple operations within a single loop. It will allow operations to appraoch the latency bound, but not lower than that. Here, two or more elements are added at a time reducing overhead (which probably involves branch prediction and something else that happens within the loop aside from termination/initiation):

```c++

void combine5  (vec_ptr v, data_t *dest){

    long i;
    long length = vec_length(v);
    long limit = length -1;
    data_t *data = get_vec_start(v);
    data_t acc = 0;

    for (i = 0; i <limit; i+=2){
        acc = (acc + data[i]) + data[i+1];
    }

    for (; i < length; i++){
        acc = acc + data[i];
    }
    *dest=acc;
}
```

In terms of machine code, combine5 does a more direct memory reference:

```assembly

i in %rdx
vmulsd  (%rax,%rdx,8), %xmm0, %xmm0
vmulsd  8(%rax,%rdx,8), %xmm0, %xmm0
addq    $2, %rdx
```

As opposed to combine4:
```assembly

data[i] in %rdx
vmulsd  %rdx, %xmm0, %xmm0
addq $8, %rdx
```
There are n operations performed with an equal number of load operations which maintains a latency bound on all operations, but the overhead is reduced by n/k


## Throughput Bounds: Exploiting Parallelism

Similar to loop unrolling, this method attemps to exploit the unused copies of functional units but simulatenously does so independantly of one another. There is no dependancy between the functional units, and therefore critical paths. The dependancy previously was found in the single source accumelator that had to be updated before the results could be added up or multiplied. Adding multiple accumelators avoids this. The code below performs an operation on the odd and even indices of the vector in parallel using two different accumelators.

void combine6  (vec_ptr v, data_t *dest){

    long i;
    long length = vec_length(v);
    long limit = length -1;
    data_t *data = get_vec_start(v);
    data_t acc0 = 0;
    data_t acc1 = 0;

    for (i = 0; i <limit; i+=2){
        acc0 = (acc0 + data[i]);
        acc1 = (acc1 + data[i+1]);
    }

    for (; i < length; i++){
        acc0 = acc0 + data[i];
    }
    *dest=acc0 + acc1;
}


*One dangerous implication of this method if splitting the inputs leads to overflow, such as the case where the majority of elements are very high on even indices, and very low on odd indices and the operation is multiplication.*.

n operations are still performed, but independandtly in two's with each iteration of the loop.

### Limitations

Two limitations are register spilling and branch prediction. Register spilling is where the number of available registers is exhausted and the CPU has to resort to memory loading and storing. The cure is to simply not assume that higher loop unrolling is automatically better and to be mindful. Branch prediciton may be fixed (if it is indeed an issue. It may not be.) by writing code that directly translates to a conditional move:


```c++
void minmax2 (long a[], long b[], long n){
    long i;
    for (i=0; i < n; i++){
        long min = a[i] < b[i] ? a[i] : b[i];
        long max = a[i] < b[i] ? b[i] : a[i];
        a[i] = min;
        b[i] = max;
    }
}
```



### Write-Read Dependancy

Not much to say except that one should avoid read-write dependancies such as the one below. The value read is dependant on a written value from the previous iteration and must wait until the result is completed and deposited in the store unit. Repeated reads or writes may be reduced by introducing an accumelator for example, as given in the bookm, or as proven in problem 5.18.

void psum1 (float a[], float p[], long n){
	long i;
	p[0]=a[0];
	for (i=0; i<n; i++)
		p[i] = p[i-1] + a[i]; 
    }


## SIMD

```c++

/*  data_t is the type,  VBYTES is the number of bytes (32 bytes for AVX2), and VSIZE is the number of elements = VBYTES/sizeof(data_t)
*/

typedef data_t vec_t __attribute__ ((vector_size(VBYTES))); // ESSENTIAL for any SIMD instructions

/* Compute inner product of SSE vector */
data_t innerv(vec_t av, vec_t bv) {
long int i;
vec_t pv = av * bv;
data_t result = 0;
for (i = 0; i < VSIZE; i++)
    result += pv[i];
return result;
}


void simd_v1_combine(vec_ptr v, data_t *dest){

long i;
vec_t accum;
data_t *data = get_vec_start(v);
int cnt = vec_length(v);
data_t result = IDENT;

/* Initialize all accum entries to IDENT */

for (i = 0; i < VSIZE; i++)
    accum[i] = IDENT;

/* Single step until have memory alignment */
while ((((size_t) data) % VBYTES) != 0 && cnt) {
    result = result OP *data++;
    cnt--;
}
    
/* Step through data with VSIZE-way parallelism */
while (cnt >= VSIZE) {
    vec_t chunk = *((vec_t *) data);
    accum = accum OP chunk;
    data += VSIZE;
    cnt -= VSIZE;
}

/* Single-step through remaining elements */
while (cnt) {
    result = result OP *data++;
    cnt--;
}

/* Combine elements of accumulator vector */
for (i = 0; i < VSIZE; i++)
    result = result OP accum[i];
/* Store result */
 *dest = result;
}

```


