# Chapter 3:Machine-Level Representation

## Worms & Viruses

The idea is to inject a piece of code somewhere and with a known address in hand, exploit buffer overflow to overwrite a return address. This would involve breaking allocated boundries of a character buffer array. 

Stack randomization combats this by randomizing the memory locations of the progrm code, library code, stack and data. The idea is that the exploit code itself will be randomly placed in memory. A nop sled is a counter-measure to this that reduces the hypothetical total address enumeration needed to crack it over a 32 or 64 bit address space.


## Stack Corruption Detection

A -randomized- canary value is pushed unto the stack immediately after a char buff array by the GCC compiler and checked before deallocating the stack space and returning. It can recur in code then, and uses the %fs special *segmented memory* read only section of the program (old, seldom used).

## Limit Executable Regions

Apparently, there used to be an additional page bit access privelage called *execute* for program code, before it was merged with the read access bit. AMD re-introduced it as NX (no-execute) for 64-bit processsors, and so did Intel. I imagine this can be used to mark regions as non-executable but readable and writable on the stack, maybe the whole stack even.

## Variable Size Frame

This section introduces the %rbp (base pointer) register: it is callee-saved, pushed  unto the stack before pointing to the bottom of the frame and staying there for the entire execution of the frame. It is used to access fixed local variables on the stack. The %rsp pointer is used to access variables of variable size on the stack. As implied, the %rbp register shows up only when there is a mix of fixed and variable size local variables on the stack to keep access to both clear and seperate. The %rbp is restore by the leave instruction which is equivalent to:

```assembly

movq %rbp, %rsp
popq %rbp

```

%rsp is restored, and rbp is popped off the stack. A return instruction following this pops the return address and deallocate the %rsp pointer by 8 (for 64-bit machines).

## Callee-saved Registers

%rbx, %rbp, %r12-r15 are callee-saved registers, meaning they must be pushed unto the stack and restored by the callee before returning to the caller.

* [rbp]: Stack Base Poiner
