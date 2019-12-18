# Chapter 7: Linking

## Object Files:

All object files are streams of contiguous bytes that carry extra information on how those bytes should be handled. That extra information is in the form of sections that define values, types, names, and addresses of the different symbols hidden within the bytes themselves. Those sections are organized into an Executable and Linkable Format (ELF) as given below: 


1.	ELF Header: word size/byte ordering/type + machine type(x86)/section header table file offset and number of entries
1.	Seaction Header Table:  Location and size of each section 
1.	.text: instruction code
1.	.data: entries for initialized global and static variables
1.	.bss: uninitialized static variables, and initialized global variables set to zero
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
## Symbol Table

The symbol table contains all the symbols that are referenced and defined by the module. These include global variables that are defined or referenced by the module, externals, and static variables and functions as well. All names are kept  in the string table section.

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

Pseudo-section exists for the section entry including COMMON, UNDEF or ABS for uninitialized global variables, externs or symbols that should not be relocated, respectively.

The linker's function ultimately is to go through the sections associating symbol references to one and only unique definition (symbol resolution) and to perform relocation if necessary when combining multiple files.

---

## Symbol Resolution:

First step in linkage involves assigning each variable to one and only one definition. Also comes the first issue in linkage of a global variable having the same name in multiple modules. The resolution is implicit and follows the following three rules:

1.	Multiple strongs lead to error
2.	One strong, multiple weaks? pick strong
3.	Multiple weaks? pick whatever

Strong variables are ones that are given a value, such as those given a value of 0 and assigned to .bss instead of COMMON.

## Static Libraries

These are simply concatenations of important and frequently used object files for use within a program. They carry an archive (.a) extension and may be created with the arr command and complied normally. Object files stored stored in the archive and invoked by other modules are copied into the executable file by the linker. The main issue is that the algorithm resolves references in order, so it is advisable to keep libraries at the very end and construct them to run in as independant fashion as possible. Libraries could also be invoked multiple times on the command line to resolve this dependency issue if needed.


## Relocation:

After copying relevant modules and symbol resolution comes relocation; the first step in relocation is in assigning the final real time addresses for all sections and defined variables. This I assume involves updating entries the symbol entry table as well. Next, comes assigning real time addresses to symbol references for each section. In short,


1.	Relocating sections: merge all setions of the same type and assign run-time memory addresses for the new merged sections and defined variables
1.	Relocating symbols: within serctions, update run-time addresses for all symbol refrences relying on relocation entries to determine symbol location, relocation type and symbol table index

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

The linker has also determined that ADDR(s) = ADDR(.text) = 0x4004d0, then the symbol sum that was originally at s + r.offset = 0xf (refptr) becomes ADDR(s) + r.offset = 0x4004df (refaddr). The value pointed to by refptr now becomes ADDR(r.symbol) + r.addend - refaddr = 0x5.


## Loading:

Finally, a program header describing the memory segments mapping to the sections and their permissions is slapped on the executable before running. There's also a new section called init and other tidbits unique to executables here:

1.	The run-time heap (allocated by malloc) grows upward, and the user stack grows downward (from 2<sup>48</sup> both sandwiching the memory mapped region for shared libraries. 
2.	An exectuable contains an init section that defines a small _init function. 
3.	A loader function handles copying everything into memory guided by the program header which contains alignment information, starting virtual address, and the entry point.
4.	The entry point is the _start function which calls the system startup function __lib_start_main which is contained in the libc.so library (always linked on compilation).
5.	The .bss section is initialized to zero at runtime.

In actuality several functions are called from the entry point defined in the elf header before the main function is ever called. The first function to be called is the \_start function defined in crt1.o which in turn calls the libc\_start\_main() function defined in lib.c. The \_init function is called after that step, the libc defined main function however is the one that handles the return code from the main user--defined function as well as passing control to the kernel if needed. All three play a role in initializing the enviroment for execution. The user--defined main function is called last.

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
