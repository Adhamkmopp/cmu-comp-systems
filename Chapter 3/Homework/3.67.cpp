typedef struct {
    long a[2];
    long *p;
} strA;

typedef struct {
    long u[2];
    long q;
} strB;

strB process (strA s){
    strB r;
    r.u[0] = s.a[1];
    r.u[1] = s.a[0];
    r.q = *s.p;
    return r;
}

long eval (long x, long y, long z){
    strA s;
    s.a[0] = x;
    s.a[1] = y;
    s.p = &z;
    strB r = process(s);
    return r.u[0] + r.u[1] + r.q;
}


/*
A.
--------------------------
24:          z
16:          &z
 8:          y
 0:          x
-8:         RET
--------------------------

B. eval passes the address of rsp + 64 for some reason (position 40 in the stack frame above)

C. via the rsp, which is odd considering that there should be an added return adress affecting the increments but it does not seem to have an effect here (24, 16 and 8 for x,y and z).

D. by filling up the the values from 64(rsp) upawrds


E.

--------------------------
48:          z
54:          x
64:          y
             .
             .
             .
24:          z
16:         &z
 8:          y
 0:          x
-8:         RET
--------------------------

F. memory management is all the way wasteful on face value. unless all these values are subject to change in the code in strA and strB, why make seperate entires for that many duplicates? Or maybe this code is just stupid (it is).
*/