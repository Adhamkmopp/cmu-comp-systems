
typedef struct {
    int x[A][B];
    long y;
} str1;

typedef struct {
    char array[B];
    int t;
    short s[A];
    long u;
} str2;

void setVal (str1 *p, str2 *q){
    long v1 = q -> t;
    long v2 = q -> u;
    p ->y = v1+v2;
}

/*
p in %rdi, q in %rsi
setVal:

movslq  8(%rsi), %rax // t in q
addq    32(%rsi), %rax // t + u
movq    %rax, 184(%rdi) // A*B = (176/4) = 44

B = 4
A = 8

That's not the correct solution I think...
*/
