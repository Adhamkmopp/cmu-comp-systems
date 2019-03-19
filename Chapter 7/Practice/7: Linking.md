#  Linking: Interestring But Badly Written

... (.cpp)> Preprocessor (ASCII)> Compiler (.s)> Assembler (relocatable object file(.o)) > Linker (executable object file)

Note:

1.	extern specifically tells the compiler that the variable is defined elsewhere
2.	systems code preceeds every executable
## The Linker:

I.	Symbol resolution: symbol references to definitions
II.	Relocation: fix addresses from relocation entires (assemlber)


ELF:

1.	ELF Header: word size/byte ordering/type + machine type(x86)/section header table file offset and number of entries
1.	Seaction Header Table:  Location and size of each section 
1.	.text: instruction code
1.	.data: entries for initialized global and static variables
1.	.bss: uninitialized static variables, and initialized global variables equal zero
1.	.symtab: functions and global variables infomration:
	1.	Global symbols: nonstatic defined
	2.	Externals: global referenced
	3.	Local symbols: static defined*
1.	.rel.text: list of locations that need to be re-addressed in  the linking process. Instructions that call external functions or reference global variables
1.	.rel.data: relocation information for global variables references or defined by module. Any initialized glboal variable that references another global vriable or external function.
1.	Pseudosections (*relocatable only*):
	1.	ABS: symbols not to be relocated
	2.	UNDEF: undefined, referenced here but defined elsewhere
	3.	COMMON: uninitialized -global, ofcourse- symbols; size is minimum, value is alignment restriction

*Local variables are kept on the stack, not on the object file. Weird.

```c

typedef struct{

	int name;	/* string table offset */
	char type: 4,	/* function or data */
	     binding:4;	/* local(static) or global */
	char reserved;	/* unused? */
	short section;	/*section header index or Pseudosection */
	long value;	/*section offset (relocatable) or absolute address (executable)*/
	long size;	/* in bytes */
} Elft64_symbol;

```

## Symbol Resolution:

1.	Multiple Symbols:
	1.	Multiple strongs lead to error
	2.	One strong, multiple weaks? pick strong
	3.	Multiple weaks? pick whatever
2.	Static Libraries: collection of binary, select and include the relocatalbe #included in order, add unresolved referenced variables to U, or resolve and add to D and select modules to add to E selectively.

## Relocation:

Relocating sections: merge all setions of the same type and assign run-time memory addresses for the new sections
Relocating symbols: within serctions, update run-time addresses for them all

Relocation entries:

```c
typedef struct{
	long offset;	/*section offset */
	long type:32.	/*relocation type like R_X86_64_PC32
	     symbol:32;	/*symbol table index*/
	long addend;	/*not always used, offset on top of offset*/
} Elf64_Rela;
```

Relocating 
```
>	foreach section s{
>		foreach relocation entry r{
>			refptr = s + r.offset;	/*ptr to reference to be relocated*/
>			/*relocate a PC-relative reference*/
>			if (r.type == R_X86_64_PC32){
>			refaddr = ADDR(s) + r.offset /*ref's runtime address*/
>			*refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr);
>		}
>		/*relocate an absolute address*/ 
>		if (r.type==R_X86_64_32)
>			*refptr = (unsigned) (ADDR(r.symbol) + r.addend);
>	}
> }
```

```c
int sum (int *a, int n);		int sum(int *a, int n)
	
int array[2] = {1,2};			int i,s =0;

int main(){			for(i=0; i<n;i++){
	int val = sum(array,2);				s+=a[i];
	return val;			}
}			return s;
		}
```


```assembly

0000000000000000 <main>:
   0:	48 83 ec 08          	sub    $0x8,%rsp
   4:	be 02 00 00 00       	mov    $0x2,%esi
   9:	bf 00 00 00 00       	mov    $0x0,%edi
			a: R_X86_64_32	array
   e:	e8 00 00 00 00       	callq  13 <main+0x13>	*with placeholder 32 bit address that will later be 0x5(below)*
			f: R_X86_64_PC32	sum-0x4
  13:	48 83 c4 08          	add    $0x8,%rsp
  17:	c3                   	retq   
```

Additionally,

```c
Elf64_Rela sum = {
	offset = 0xf;	/*section offset */
	type = R_X86_64_PC32;	/*relocation type like R_X86_64_PC32
	symbol = sum;	/*symbol table index*/
	addend = 0x4;	/*not always used, offset on top of offset*/
} Elf64_Rela;
```

The linker has also determined that ADDR(s) = ADDR(.text) = 0x4004d0, then the symbol sum that was originally at s + r.offset = 0xf (refptr) becomes ADDR(s) + r.offset = 0x4004df (refaddr). The value pointed to by refptr now becomes ADDR(r.symbol) + r.addend - refaddr = 0x5.


## Loading:

There isn't much here that's noteworthy aside from  the fact that the run-time heap (allocated by malloc) grows upward, and the user stack grows downward (from 2<sup>48</sup> both sandwiching the memory mapped region for shared libraries. A Loader function handles copying everything into memory guided by the program header.

## Dynamic Linking:

Shared libraries (DLLs, finally, mystery solved) are loaded into arbritrary memory location by a dynamic linker along with the partially linked code of our program, then the linking is performed before passing control back to the loader which then initiates the program at the entry point. They are shared by multiple processees at once.

linux> gcc -shared -fpic -o libvector.so addvec.c multvec.c
linux> gcc -o prog21 main2.c ./libvector.so

No code is actually copied into the exectuable, but the relocations tables and symtables is copied to allows resolving of references from libvector at runtime. The dynamic loader then relocates the code from libvector.so to some memory segment, and resolves all references. I don't know if that means that libvector is in and of itself a reloctable binary object, or if it's copied in memory, or if it's eligable for use by any and all other processees. This is a badly written chapter about an already confusing topic. Shame.

This is not the same as actually linking *dynamically* (what is this bullshit?) through the application after it has begun execution.

```c
#include <dlfcn.h>

void *dlopen(const char *filename, int flag); /* flag being RTLD_GLOBAL, RTLD_NOW or RTLD_LAZY*/
void *dlsym(void *handle, char *symbol); /* returns address to a symbol in the handle (shared library above) */
```

## Position Independant Code

Code segments are dropped anywhere in memory to be shared by whoever.
