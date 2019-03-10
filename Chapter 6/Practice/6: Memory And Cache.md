![Some kind of way of dealing with the mass of information in this chapter](https://raw.githubusercontent.com/Adhamkmopp/cmu-comp-systems/master/Extras/ch6.jpg)


This is not a particularly difficult chapter, but it is loaded with information. The main bulk of it is informational; bus design, memory design, cache design and so on with little in the way of improving coding performance. As such, a quick visual summary above was more than sufficient for a complete overview of the chapter, alongwith some key terms:


*Memory Mapped I/O: reserved address space for devices (each, a port).
*Good locality: which includes temporal (how often a single variable is accessed for read or write), and spatial (how well is key for exploiting cache availability.
*Cache miss: a victim block is evicted following a block replacement policy (Least Recently Used seems to be a fan favorite).
*Cache placement: not random, too expensive, organized placement in some way. 
*Logical Disk Block: translation into actual (surface, track, sector) triplets handled by the disk controller firmware for the OS.
**Formatting: filling gaps with sector IDs/ designating spare cylinders.

Miss Types:

    I.  Cold Miss: empty cache
    II. Conflic Miss: block missed due to conflicting block placement when one restricts block placement to non-random (can lead to trash metal slowdown by a factor of 2, even 3)
    III.    Capacity Miss: working set size too big for the cache

Write Policies:

    I. Write-Back: update cache alongwith with a dirty bit and defer till block eviction to transfer block down to a lower level, increasing complexity.
    II. Write-Allocate: for dealing with a write-miss, load block from lower level and update instead of updating lower level directly. Exploit spacial locality, but increase bus traffic/miss penalty.
    III. Write-Through: write directly down a lower level, possiblty increasing traffic.
⋅⋅*⋅⋅*
