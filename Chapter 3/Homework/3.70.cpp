union ele {
    struct{
        long *p;
        long y;
    } e1;
    struct{
        long x;
        union ele *next;
    } e2;
};

void proc (union ele *up){
    up -> e2.x = *(*(up -> e2.next). e1.p) -  *(up->e2.next).e1.y   ;
}

/*

A.

e1.p    =   0
e1.y    =   8
e2.x    =   0
e2.next =   8

B. 8
C.


up in rdi
    movq    8(rdi), rax //  e2 -> next or e1.y dereferenced and stored in rax
    movq    (rax), rdx  // dereference again so it's definetly up -> e2 -> (next)
    movq    (rdx), rdx  // dereference yet again so it's definetly up -> e2 -> (next) -> e1.p 
    subq    8(rax), rdx // up -> e2 -> next -> e1.y
    movq    rdx, (rdi)
    ret

*/

