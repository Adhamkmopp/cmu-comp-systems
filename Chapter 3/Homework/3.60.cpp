long loop(long x, int n)
{
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask = mask << 1) {
        result |= (mask & x);
    }
    return result;
}

/*

long loop(long x, int n)
x in %rdi, n in %esi

loop:
    movl    %esi, %ecx      # n in %rcx
    movl    1, %edx         # 1 in %rdx
    movl    0, %rax         # 0 in %rax
    jmp     .L2
.L3:
    movq    %rdi, %r8      # x in %r8
    andq    %rdx, %r8      # mask AND x in %r8
    orq     %r8, %rax      # rax = rax | %r8
    salq    %cl, %rdx      # %rcx << %rdx
.L2:
    testq   %rdx, %rdx      # does not equal zero
    jne     .L3
    rep; ret 

A. mask in rdx, result in rax
B. result is 0, mask is 1
C. mask does not equal 0
D. mask is shifted to the left (multiplied by 2)
E. result is OR with mask AND x
F. done but what does it do? clusmily makes a copy of x bit by bit