typedef struct {
    int first;
    a_struct a[CNT];
    int last;
} b_struct;

void test(long i, b_struct *bp)
{
    int n = bp ->first + bp ->last;
    a_struct *ap = &bp -> a[i];
    ap->x[ap->idx] = n;
}

/*
mov     0x120(%rsi), %ecx // a_struct a[CNT] is 120-4 = 11C (284 bytes) large
add     (%rsi), %ecx
lea     (%rdi, %rdi,4), %rax // 5i
lea     (%rsi, %rax,8), %rax // 40i + rsi in rax, each a_struct is 40 bytes.
mov     0x8(%rax), %rdx // (8 + 40i + rsi) in rdx, "idx"
movslq  %ecx, %rcx
mov     %rcx,0x10(%rax,%rdx,8) // 8*(8 + 40i + rsi) + (40i + rsi) + 10 
retq

A. CNT is 288 - 8 / 40 = 7
B. No fucking idea. 

The adress of idx is (8 + 40i + rsi) for each i out of 7.
We need to dereference idx, multiply it by 8 and then add it to the address of x to get the correct address.
Then we need a final dereferencing to store n in said address. This is done in one step:

%rcx,0x10(%rax,%rdx,8) // 8*(8 + 40i + rsi OR "idx") + (40i + rsi + 16) Then a_struct is:

typedef struct {
    long idx;
    long x[2];
} a_struct;

*/