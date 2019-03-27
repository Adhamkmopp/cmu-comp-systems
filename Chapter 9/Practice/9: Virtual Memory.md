## Terminology & Mechanisms

The main idea is that contiguous sections of virtual pages are allocated whereby each virtual page corresponds to some physical part of the main memory scattered randomly; a file on disk for example may be allocated by dividing the file into pages, and then associating a contiguous section of the virtual address space with the file pages. If a page from the file is cached in main memory then the page from the disk is "swapped/paged in". 

Physical Address: Each byte in memory has a unique address.
Virtual Address: a virtual address is translated into a main memory physical address first and corresponds to a space in memory.

Each memory byte has dual address; a phycial and a virtual one. On disk, space is partitioned as a blocks of virtual  pages of P = 2 <sup>p</sup> size which may be unallocated, cashed or uncached. DRAM caches are write-back and keep large sized blocks/pages (4 KB to 2 MB) to offset a higher miss penalty than the SRAM cache. It is also fully associative. To cache virtal page, a page table maintained by the kernel contains all page entires stored on a disk and accessible through the virtual address as an index. Each entry either points to the phycial address on the disk if uncached, or -part of- the physical address on RAM if it is as indicated by the valid bit. A cache hit is handled by the kernel through a fault exception handler (a page fault handler specifically), including impelmenting a replacement policy for picking a victim page, and performing write-backs if necessary, not just simply caching the page.

Every process gets its own page table and private address space on memory alongwith additional bits set for permissions. Every page table entry (PTE) is 32 to 64 bits in total, with the physical address occupying only a part of the whole entry.

## Address Translation

A virtual address consists of: 
1.  Virtual Page Offset (p-1 bits): concatenated alongwith the PTE "physical address" (Physical Page Number) to give the physical address.
2.  Virual Page Number (n - p bits): this is the PTE index.


The processor generates a virtual address and sends it to the MMU which generates a PTE addres and requests it from the cache, the cache returns the PTE to the MMU which is used to construct a physical address. In the case of a miss,the MMU triggers an exception and the fault handler takes care of the rest.

An additional cache called the Translations Lookaside Buffer(TLB) where each line stores a PTE may be utilized to speed things up. Fetching the PTE in this case may be ommited from the main memory, where it may take hundreds of cycles.

## Multi-level Pages

To reduce memory consumption, PTE may be organized into chunks in a level k page table. The page table may be set to null if none of the chunks has allocated pages, and if it does, then it points to a level k - 1 page table that in turn points to a lower level page table, and so on until a single level page table is reached. Null chunk entries need not exist, and lower level may not even need to reside in memory (unless heavily referenced), but should and can be created on the fly as the need arises.

---
## Algorithm & Example

First, armed with a virtual address the MMU extracts the VPN and sends the address to the TLB (The TLB index and tag overlap with the VPN), and similarly sends the VPO to the L1 cache to prepare incase a hit had occured (The CO and CI overlap with the VPO). If there's a hit, the PPN is retrieved from the TLB to construct a physical address. If there's a miss, the MMU repeats the process with the main memory's page table.

An Intel Core i7/Linux system supports 48-bit and 52-bit virtual and physical address spaces respectively. There are four levels of page tables where each entry is 64-bits carrying a dirty bit, reference bit (for replacement policies), write-through or write-back bit, permissions bits, and 40-bit page table physical address. The VPN (36-bits and the VPO is 12-bits) is split four equal ways and each secion is used as a PTE index as follows: a CR3 (context dependant) base register points to the first level page table, while the VPN determines the offset. Each entry points to another page table and so on, until the final address of the page in quesiton.

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

Linux keeps a task_struct that carries some essential process information including an mm entry that contains a pointer to a mm_struct that in turn has a pgd entry (pointer to first level page tables - loaded into CR3) and an mmap entry that contains a pointer to vm_area_struct that carries pointer entries to the different start and end areas/segments in VM.

## Memory Mapping

File secions are mapped into memory areas consisting of a collection of contiguous pages (if the area is larger than a section then it is padded with zeros). Shared objects are placed in a *shared area*. A private object's page table entries are intially read-only and the area is flagged as private copy-on-write. IF a process attempts to write to some page in that private area, a fault protection is triggerd which copies the area, updates the page physical pointer entry and restrores write privelages to the process. Copying is deferred to the last possible moment.

The memory area types may be:
1.  The user stack: private, demand-zero of zero length
1.  Shared libraries: shared and file-backed
1.  Run-time heap: private, demand-zero of zero length
1.  Unitialized data (.bss): private, demand-zero of zero length
1.  Initialized data: private, file-backed
1.  Code (.text): private, file-backed

This is what occurs with the fork() function; copies are made of the task_struct, mm_struct and vm_area_struct and page tables. The copy-on-write triggers ensure a private address space for each process.


## Dynamic Memory Allocation

The heap may be allocated with malloc which uses mmap and munmap, or it could use the sbrk function:

'''c
void *sbrk(intptr_t incr);
'''

The sbrk function increments the brk pointer which always points to the top of the heap, and returns the old brk value. The free function however takes in the pointer to the beginning of the block to be freed. This implies an internal mechanism keeping track of block sizes (the header most likely).

An allocator must operate within strict constraints:

1.  Arbitary request sequences
2.  Immediate responses to requests
3.  Block alignment
4.  Non-modifying allocated blocks

In the end, a good allocator compromises throughput to acheive better memory utilization.

## Fragmentation & Implementation

1.  Internal: the allocated block is larger than the payload.
2.  External: enough aggregate memory, not enough contiguous memory

There are issues to keep in mind with implementation:

1.  Free block organization: keeping track of free blocks
2.  Placement: where to place a newly allocated block
3.  Splitting: what to do with the remainder of free blocks
4.  Coalescing: what to do with a newly freed block

Implicit Free Lists

Logic: store a one-word header (32-bits) at the beginning of a block denoting the size of the block, and whether it is allocated or free with the 3 lower order bits, followed by the payload and padding if necessary. Traversing the chain of blocks in this way is a way of jumping to free blocks.