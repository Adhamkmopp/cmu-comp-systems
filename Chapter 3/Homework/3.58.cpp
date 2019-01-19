long decode2(long x, long y, long z){
    long result;
    result = (z - y) * x;
    result <<= 63;
    result >>= 63;
    return (x ^ result);

}

int main (){
    long end = decode2(1,2,3);
}

/*

00000000000005fa <_Z7decode2lll>:
 5fa:	55                   	push   %rbp
 5fb:	48 89 e5             	mov    %rsp,%rbp
 5fe:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
 602:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
 606:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
 60a:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
 60e:	48 2b 45 e0          	sub    -0x20(%rbp),%rax
 612:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
 616:	48 0f af c2          	imul   %rdx,%rax
 61a:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 61e:	48 c1 65 f8 3f       	shlq   $0x3f,-0x8(%rbp)
 623:	48 c1 7d f8 3f       	sarq   $0x3f,-0x8(%rbp)
 628:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
 62c:	48 33 45 f8          	xor    -0x8(%rbp),%rax
 630:	5d                   	pop    %rbp
 631:	c3                   	retq   

**/