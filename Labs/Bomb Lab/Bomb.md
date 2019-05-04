## Phase 1:

"Border relations with Canada have never been better."

Easily solved by checking out the string at the address used as a condition.

## Phase 2

"1 2 4 8 16 32"

First by looking at the string parameter fed into scanf, there were six decimals ("%d"). Next, a register was added to itself and checked against each element returned by scanf for equality, starting from 1. Which would be the sequence above.


## Phase 3

"1 311"

Solved by looking into the register values used in checking equality signs (I can't remember my specific steps unfortunately, but it was not *mathematically* sensible on my part. I remember there being 6 distinct possiblities for the first digit from 0 to 6, and a second associated number checked against it.)

## Phase 4

"7 0"

The func4 function recursively calls itself, checking 7, 3 , 1 and 0 against the first digit and unwinding if it is less than or equal to it. If the number was infact equal to 7,3,1 or 0 (a second greater than or equal jump after the first) then the register rax would remain at 0, which was a necessary condition for defusing the bomb. The final condition was a check of 0 against the second number, which finally defused the bomb if true.


## Phase 5

"ionefg"

Phas 5 is roughly equivalent to:


```c
char arr_input[6] = read_line();
char new_word[6];
char *y;

for (i= 0; i <6; i++){
    *y = arr[i]; 
    *y  = *y >> 4;
    new_word[i] = global_arr[(int) *y];
}
```

The array index however is a single hexadecimal (4 bits), not an explicit decimal. The global array is: "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?". 

## Phase 6

"4 3 2 1 6 5".

Each 6 n<sub>i</sub> is unique and has to be lower than 6, then it is transformed by n<sub>i</sub> = 7 - n<sub>i</sub>. It took me a long while to figure out that the nodes were a linked list (with the help of online sources). What was confusing were two things: the difference between the lea and mov instructions at lines 4011ab to 4011b5. I could not see right away that one was a memory address, and the other was a code address. It was necessary as one code address had to be access and changed with the other code address of the next node stored on the stack. The other thing infact that the linked list elements could be stored in that way, and access with (gdb) x/3x to see all 3 of its elements. Node values are finally access and compared on a descending order basis.


```assembly

00000000004010f4 <phase_6>:


% 0x6032d0 in %rdx, 1 in %rax , 0x7fffffffdcc0 in %rsp (transformed digits' start), 0 in %rsi, % rcx =  n<sub>i</sub> %

  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>

    ---
    // beg loop
    401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx // increment rdx by 8
    40117a:	83 c0 01             	add    $0x1,%eax
    40117d:	39 c8                	cmp    %ecx,%eax //check if numbers equal
    40117f:	75 f5                	jne    401176 <phase_6+0x82>
    // end loop

    ---
    401181:	eb 05                	jmp    401188 <phase_6+0x94>
    401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx

    401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2) // store rdx in 20 + rsp + rsi*2
    40118d:	48 83 c6 04          	add    $0x4,%rsi //rsi increases by 4
    401191:	48 83 fe 18          	cmp    $0x18,%rsi // check if at end of array
    401195:	74 14                	je     4011ab <phase_6+0xb7>

    401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx // new number
    40119a:	83 f9 01             	cmp    $0x1,%ecx // compare with 1
    40119d:	7e e4                	jle    401183 <phase_6+0x8f>
    40119f:	b8 01 00 00 00       	mov    $0x1,%eax
    4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx
    4011a9:	eb cb                	jmp    401176 <phase_6+0x82>

    ---

    % After the loop. Mode address in order stored in rax and rbx %

  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx // beginning of node addresses
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax // next node
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi // no idea. boundry?
  4011ba:	48 89 d9             	mov    %rbx,%rcx
  4011bd:	48 8b 10             	mov    (%rax),%rdx
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx) // let the first node point to the node after
  4011c4:	48 83 c0 08          	add    $0x8,%rax
  4011c8:	48 39 f0             	cmp    %rsi,%rax
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>
  4011cd:	48 89 d1             	mov    %rdx,%rcx
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)
  4011d9:	00 
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax
  4011e3:	8b 00                	mov    (%rax),%eax // get value of node into rax
  4011e5:	39 03                	cmp    %eax,(%rbx) // compare previous value with next
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>
  4011e9:	e8 4c 02 00 00       	callq  40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>
  4011f7:	48 83 c4 50          	add    $0x50,%rsp
  4011fb:	5b                   	pop    %rbx
  4011fc:	5d                   	pop    %rbp
  4011fd:	41 5c                	pop    %r12
  4011ff:	41 5d                	pop    %r13
  401201:	41 5e                	pop    %r14
  401203:	c3                   	retq   
```


6 : 0x603320
5 : 0x603310
4 : 0x603300
3 : 0x6032f0
2 : 0x6032e0
1 : 0x6032d0