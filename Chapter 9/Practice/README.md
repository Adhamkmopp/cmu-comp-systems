# Chapter 9: Virtual Memory

## Terminology & Mechanisms

The main idea is that contiguous sections of virtual pages (akin to cache blocks) on the disk are allocated whereby each virtual page corresponds to some physical part of the main memory scattered randomly; a file on disk for example may be allocated by dividing the file into pages, and then associating a contiguous section of the virtual address space with the file pages. If a page from the file is cached in main memory then the page from the disk is "swapped/paged in". 

Physical Address: Each byte in memory has a unique address.
Virtual Address: a virtual address is translated into a main memory physical address first and corresponds to a space in memory.

Each memory byte has dual address; a phycial and a virtual one. On disk, space is partitioned as a blocks of virtual pages of P = 2 <sup>p</sup> size which may be unallocated, cashed or uncached. DRAM caches are write-back and keep large sized blocks/pages (4 KB to 2 MB) to offset a higher miss penalty than the SRAM cache. It is also fully associative (one set, no set bits). To cache virtal page, a page table maintained by the kernel contains all page entires stored on a disk and accessible through the virtual address as an index. Each entry either points to the address of the page on the disk if uncached, or points to -part of- the physical address on DRAM as indicated by the valid bit. An unallocated page is NULL. A cache hit is handled by the kernel through a fault exception handler (a page fault handler specifically), including impelmenting a replacement policy for picking a victim page, and performing write-backs if necessary, not just simply caching the page.

*NOTE*: the memory available to a process is capped at the 32-bit or 64-bit address limit, regardless of actual DRAM capacity.


Every process gets its own page table and private address space on memory alongwith additional bits set for permissions. Every page table entry (PTE) is 32 to 64 bits in total, with the physical address occupying only a part of the whole entry.

## Address Translation

A virtual address consists of: 
1.  Virtual Page Number (n-p bits): The PTE index and set/tag access for TLBs
1.  Virtual Page Offset (p-1 bits): concatenated alongwith the PTE "Physical Page Number" to give the actual physical address.


The processor generates a virtual address and sends it to the MMU which generates a PTE addres and requests it from the cache, the cache returns the PTE to the MMU which is used to construct a physical address. In the case of a miss,the MMU triggers an exception and the fault handler takes care of the rest.

An additional cache called the Translations Lookaside Buffer(TLB) where each line stores a PTE may be utilized to speed things up. Fetching the PTE in this case may be ommited from the main memory, where it may take hundreds of cycles.

## Multi-level Pages

To reduce memory consumption, PTE may be organized into chunks in a level k page table. The page table may be set to null if none of the chunks has allocated pages, and if it does, then it points to a level k - 1 page table that in turn points to a lower level page table, and so on until a single level page table where the entries are actual PPNs of physical disk blocks is reached. Null chunk entries need not exist, and lower level may not even need to reside in memory (unless heavily referenced), but should and can be created on the fly as the need arises. All in all, the TLBs caching of PTE's makes it not significantly slower than single entry page tables, but it is slower, and that is the offset of size vs speed.

*NOTE*: accessing a page requires access to k pages.

---
## Algorithm & Example

First, armed with a virtual address the MMU extracts the VPN and sends the address to the TLB (The TLB index and tag overlap with the VPN), and similarly sends the VPO to the L1 cache to prepare incase a hit had occured (The CO and CI overlap with the VPO). If there's a hit, the PPN is retrieved from the TLB to construct a physical address. If there's a miss, the MMU repeats the process with the main memory's page table.

An Intel Core i7/Linux system supports 48-bit and 52-bit virtual and physical address spaces respectively. There are four levels of page tables where each entry is 64-bits carrying a dirty bit, reference bit (for replacement policies), write-through or write-back bit, permissions bits, and 40-bit page table physical address. The VPN (36-bits and the VPO is 12-bits) is split four equal ways and each secion is used as a PTE index as follows: a CR3 (context dependant) base register points to the first level page table, while the VPN determines the offset. All page tables of allocated pages in ever present in memory.

1.  memory is byte addressable
1.  memory accesses are 1-byte words
1.  virtual addresses are 14 bits
1.  physical addresses are 12 bits
1.  page size is 64 bytes
    2.  256 entries (2<sup>14 - 6 = 8</sup>)
    2.  VPO: bits 0 to 5
    2.  VPN: bits 6 to 13
1.  TLB is 4-way associative with 16 total entries
    2.  4 sets, 4 lines
    2.  TLBI: bits 6 to 7
    2.  TLBT: bits 8 to 13
1.  L1 cache physically addressable and direct mapped with 4 byte line and 16 total sets
    2.  one line per set, 64 bytes total
    2.  CO: bits 0 t0 1
    2.  CI: bits 2 to 4
    2.  CT: bits 5 to 11
---

### Practice problem 9.4:

A.  00 0011 11 010 111
B.

Parameter   Value
VPN 0x0f
TLB index   3
TLB tag 0x03
TLB hit y
Page fault  n
PPN 0x0d

C.     0011 0101 0111 = 0x357

D.
Parameter   Value
Byte offset 3
Cache index 5
Cache tag   0x0d
Cache hit   y
Cache byte returned 1D


## Linux Memory Organization

Linux keeps a task_struct that carries some essential process information:

1.  user stack pointer
2.  PID
3.  Name of executable file
4.  mm_struct pointer:
    1.  pgd: level-1 table base pointer (stored in CR3)
    2.  mmap pointer: points to base of vm_area_struct:
        1.  vm_prot: permissions for *pages*
        2.  vm_flags: private or shared *areas*
        3.  vm_start/vm_end: boundries
        4.  vm_next: the areas link together, it seems.



Before any page access request is made, a check through all the areas (superimposed tree) is done to see if it falls within the start/end addresses of any. Any page falling outside any of the designated areas, or without the proper permissions throws a page fault, despite the fact that the Core i7 provides stringent and exhaustive supervision on the level of individual pages on each PTE. 

## Memory Mapping

File-backed secions are mapped into memory areas consisting of a collection of contiguous pages on a physical disk (if the area is larger than a section then it is padded with zeros). Anonymous files are still a mystery, but it apparently includes the stack and the heap so maybe the purpose is to reserve space. Fictional space in a magical realm that only exits as a concept, like Limbo. Shared objects are placed in a *shared area*,private objects are placed in a private area but may still be mapped to different processes at the same time, which is a contrived way of saying "shared, but of a differnt kind". A private object's page table entries are intially read-only and the area is flagged as private copy-on-write. If a process attempts to write to some page in that private area, a fault protection is triggerd which copies the area, updates the page physical pointer entry on for the offending process and restrores write privelages to said process. Copying is deferred to the last possible moment. This is a combination of control between the Core i7 permissions control, and Linux's own area permissions control.

The memory area types may be:
1.  The user stack: private, demand-zero of zero length
1.  Shared libraries: shared and file-backed
1.  Run-time heap: private, demand-zero of zero length
1.  Unitialized data (.bss): private, demand-zero of zero length
1.  Initialized data: private, file-backed
1.  Code (.text): private, file-backed

## Fork()

Similar to the mechanism inplace for private copy-on-write, a fork() call copies the mm_struct (which includes a new PID), sets each *page* as read-only and each area as private copy-on-write. Voila.

## Area Allocation

```c
void *mmap(void *start, size_t length, int prot, int flags, int fd, off_set offset);
```

fd is the open file descriptor apparently. Prot and flags are set as macros (PROT_READ/WRITE/EXEC/NONE, MAP_SHARED,MAP_PRIVATE, MAP_ANON). bufp is set to the new address that need to be used when deleting the area itself.

## Dynamic Memory Allocation

The heap may be allocated with malloc which may use mmap and munmap itself, or it could allocate it dynamically with sbrk; the heap is a reserved demand-zero area that typically begins after the data section and grows upwards. The process maintains a brk variable that points to the top of the heap. Heap is allocated in blocks using mallloc and free which operate much in the same way as new and delete in C++.

The sbrk function explicitly allocates heap by increasing the brk pointer value:

```c
void *sbrk(intptr_t incr);
```

The sbrk function increments the brk pointer which always points to the top of the heap, and returns the old brk value. The free function however takes in the pointer to the beginning of the block to be freed. This implies an internal mechanism keeping track of block sizes (the header most likely).

*NOTE*: the sbrk returns the old brk pointer, which is actually the *epilogue* block that will be transformed into a new header block by some other function in the case of implicit lists.

An allocator must operate within strict constraints:

1.  Arbitary request sequences: free and allocate in any order
2.  Immediate responses to requests: no buffering or queing
3.  Block alignment: for any data type
4.  Non-modifying allocated blocks: no moving around blocks

In the end, a good allocator compromises throughput to achieve better memory utilization (peak utilization).

## Fragmentation & Implementation

1.  Internal: the allocated block is larger than the payload; minimum block size restriction
2.  External: enough free aggregate memory, not enough contiguous memory

There are issues to keep in mind with implementation:

1.  Free block organization: keeping track of free blocks
2.  Placement: where to place a newly allocated block
3.  Splitting: what to do with the remainder of free blocks
4.  Coalescing: what to do with a newly freed block

##  Implicit Free Lists (block organization)

(keeping in mind that malloc returns blocks aligned by multiples of 8 or 16) Blocks keep track of their size in a 4 bytes header (32 bits), of which the last 3 bits are always zero due to alignment. The last bit in this case may be set freely to 1 or 0  o indicate status as free or allocated.

Searching for a free block requires traversing the entire list which is costly, but rewarding for its simplicity. Furthermore, internal fragmentation is kept at a minimum compared to other methods as the only restrictions in effect are the alignment restriction, and the inclusion of a header on each block.

##  Block Placement Policy

1.      First fit: Allocated a block when first encountered. Has the advantage of retaining large blocks towards the end of the heap, but can introduce alot of "splinters" (external fragmentation?) that further increase the search time for thos coveted large blocks in the first place

2.      Best fit: Exhaustively search all free blocks for the smallest fit block. Negates external fragmentation as much as possible, but is terrible time wise. Strategies exists to offset the cost however.

3.  Next Fit: continue searching for a free block where the last search left off. Faster than first fit, but with worse memory utilization. Smack in the middle between best and first fit.
##      Coaslescing Free Blocks

Free blocks that were introduced after splitting and freeing a block can introduce *false fragmentation*; it basically means that two or more adjacent blocks are free but seperate, leading to a potential situation of external fragmentation. It can be combatted in 2 ways:

1.      Immediate coalescing: adjacent blocks are immediatly coaslced after freeing a block
2.      Deferred coalescing: usually faster but introduces a different kind of overhead where the entire heap is searched and all free blocks are coalesced and only after some allocation request fails.


The implementation however is tricky. A block that is about to be freed can easily check if the next block is free or not and to coalese. Checking a previous block would require searching the entire heap of blocks to reach the previous ones' header. A fix would be to include a footer with size and status information that is always a word away from the current block's header. An even better fix is to omit footers in allocated blocks to save up on memory usage, and to uitilize the extra free bits in the next blocks header instead. The footers would only exist on free blocks in this case.

## 


## Garbage Collectors

Based on the idea of a reachability directed graph where the root contains some pointer to the graph of allocated blocks (may be a register or a global variable). The garbage becomes unreachable when this root is abolished somehow such as the case in a function return without deploying free(). Lanugages like C or C++ may call a conservative collector to search for unreachable nodes and call free where appropriate, because C/C++ does not maintain reachability graphs very well (an odd statment, but that is essentially what was communicated in the book).


The Mark&Sweep approach is called for each root node and it searches recursively along with length of the block for pointers to other blocks, marking each block as "reachable" along the way. A sweep phase searches along the entire heap for unmarked "unreachable" blocks that are then considered garbage and freed. The only reason why this must be conservative in C is because no type information is associated with pointers, and an int may masquerade as a pointer when infact it is not. A balanced binary tree adds additional information in the header of each block to construct a binary tree of reachable blocks from the root, and may be deployed to counteract the type issue.