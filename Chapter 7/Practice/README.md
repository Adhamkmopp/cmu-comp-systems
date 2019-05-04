# Chapter 7: Linking

## The Linker:

All object files are streams of contiguous bytes that carry extra information on how those bytes should be handled. That extra information is in the form of sections that define values, types, names, and addresses of the different symbols hidden within the bytes themselves. Those sections are organized into an Executable and Linkable Format (ELF) as given below: 


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
	3.	COMMON: uninitialized -global, ofcourse- symbols; size is minimum, value is alignment restriction. COMMON sections hold weak values as  measure of deferrance on which will be chosen.

*Local variables are kept on the stack, not on the object file.

Below is the structure of a symbol entry in the symbol table. The name is a byte offset into the string (.strtab) section, and the value is a byte offset into the section where the symbol is defined, or as an absolute run-time address if on an exectuable file. 

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

The linker's function ultimately is to go through the sections associating symbol references to one and only unique definition (symbol resolution) and to perform relocation if necessary when combining multiple files.

---

## Symbol Resolution:

1.	Multiple Symbols:
	1.	Multiple strongs lead to error
	2.	One strong, multiple weaks? pick strong
	3.	Multiple weaks? pick whatever


## Static Libraries

These are simply concatenations of important and frequently used object files for use within a program. They carry an .a extension and may be created with the *arr rcs lib.a obj1.o obj2.o* command and complied with the *gcc -static prog2c main2.o ./lib.a* command. The main issue is that the algorithm resolves references in order, so it is advisable to keep libraries at the very end and construct them to run as independantly as possible.


## Relocation:

1.	Relocating sections: merge all setions of the same type and assign run-time memory addresses for the new merged sections
1.	Relocating symbols: within serctions, update run-time addresses for them all relying on relocation entries

## Relocation entries:

```c
typedef struct{
	long offset;	/*section offset of the reference to relocate */
	long type:32.	/*relocation type like R_X86_64_PC32 */
	     symbol:32;	/*the symbol entry name where the reference is actually defined*/
	long addend;	/*not always used, offset on top of offset*/
} Elf64_Rela;
```

## Relocating Algorithm:
ADDR(r.symbol) is the run-time address of the function or variable, where it is defined.ADDR(s) is the run-time address of a section.


```
>	foreach section s{
>		foreach relocation entry r{
>			refptr = s + r.offset;	/*ptr to reference to be relocated. all 0's initially*/
>			/*relocate a PC-relative reference*/
>			if (r.type == R_X86_64_PC32){
>			refaddr = ADDR(s) + r.offset /*ref's runtime address i.e. where it is referenced */
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

## Relocation Structs

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

1.	The run-time heap (allocated by malloc) grows upward, and the user stack grows downward (from 2<sup>48</sup> both sandwiching the memory mapped region for shared libraries. 
1.	An exectuable contains an init section that defines a small _init function. 
1.	A Loader function handles copying everything into memory guided by the program header which contains alignment information, starting virtual address, and the entry point.
1.	The entry point is the _start function which calls the system startup function __lib_start_main which is contained in the libc.so library (always linked on compilation).
1.	The .bss section is initialized to zero at runtime.

## Dynamic Linking:

Shared libraries (DLLs, finally, mystery solved) are loaded into arbritrary memory location by a dynamic linker along with the partially linked code of our program, then the linking is performed before passing control back to the loader which then initiates the program at the entry point. They .text section is shared by multiple processees at once.

No code is actually copied into the exectuable, but the relocations tables and symtables is copied to allows resolving of references from libvector at runtime. The dynamic loader then relocates the code from libvector.so to some memory segment, and resolves all references.

Libraries can also be linked *during* runtime using an in-text C function:


```c
#include <dlfcn.h>

void *dlopen(const char *filename, int flag); /* flag being RTLD_GLOBAL, RTLD_NOW or RTLD_LAZY*/
void *dlsym(void *handle, char *symbol); /* returns address to a symbol in the handle (shared library above) */
```
The function dlopen loads and links a dynamic library with options on when to perform symbol resolution. The second function returns a pointer to a symbol.


## Library Interpositioning

Code segments are dropped anywhere in memory to be shared by whoever. The code segment is read only and references data by a relative jump into the data section's global offset table (GOT), which in turn is relocated once a library is built.
