typedef __int128 int128_t;

void store_prod (int128_t *dest, int64_t x, int64_t y){
    *dest = x * (int128_t) y;
}

/*

store_prod:

*dest in rdi, x in rsi and y in rdx

movq    %rdx, %rax      # y in %rax
cqto                    # y in %rax::%rdx
movq    %rsi, %rcx      # x in %rcx
sarq    $63, %rcx       # sign_x in %rcx
imulq   %rax, %rcx      # (sign_x * y) in rcx ( lower 64 bits )
imulq   %rsi, %rdx      # (sign_y * x) in rdx
addq    %rdx, %rcx      # (sign_x * y + sign_y * x) in rcx
mulq    %rsi            # x * y in %rax::%rdx
addq    %rcx, %rdx      # (((sign_x * y) + (sign_y * x)) + (higher bit) (x * y)) in rdx
movq %rax, (%rdi)       # lower 64-bits as x *y from the mulq operation
movq %rdx, 8(%rdi)      # upper 64-bits as (((sign_x * y) + (sign_y * x)) + (higher bit) (x * y))
ret 


 ux * uy = (2^64*x_h + x_l) * (2^64*y_h + y_l)
       = 2^128*(x_h)*(y_h) + (x_l)*(y_l) + (2^64)(x_l)(y_h) + (2^64)(x_h)(y_l)
       = (x_l)*(y_l) + (2^64)(x_l)(y_h) + (2^64)(x_h)(y_l)
       
I am more than a little confused by the equations above and the operations in the assembly code. My only guess as to what is going on
is that the inclusion of sign_x or y multiplied by the other is done to convert to a signed multiplication somehow as a consequence
of using a full unsigned multiplication (mulq).
*/