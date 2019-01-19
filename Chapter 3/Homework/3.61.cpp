

int cread_alt(int *xp)
{
    static const int zero = 0;
    return (!xp ? 0 : *xp);
}

int main(){
    int *x;
    cread_alt(x);
}


/*

From:https://stackoverflow.com/questions/26310555/how-to-create-a-function-using-conditional-data-transfer

int cread_alt(int *xp)
{
    static const int zero = 0;
    return *(xp ? xp : &zero);
}

Cleverly avoids dereferencing until the very end. However, the code also indiciates that it will be invalid:

 5fa:	55                   	push   %rbp
 5fb:	48 89 e5             	mov    %rsp,%rbp
 5fe:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 602:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)

The above code is also incorrect. It is odd how little control I have over my own code, and how much of it is in the hands of a compiler

*/