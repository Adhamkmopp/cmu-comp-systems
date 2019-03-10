![Some kind of way of dealing with the mass of information in this chapter](https://raw.githubusercontent.com/Adhamkmopp/cmu-comp-systems/master/Extras/ch6.jpg)

What Makes A Program?

Good locality, which includes temporal (how often a single variable is accessed for read or write), and spatial (how well organized the data is as to gurantee its existence in a lower level memory block (lower stride in general is better)).




Block Properties

Fixed size between adjacent levels, variable size between upper and lower levels.

Cache miss: a victim block is evicted following a block replacement policy (Least Recently Used seems to be a fan favorite).
Cache placement: not random, too expensive, organized placement in some way. 
Types:

    I.  Cold Miss: empty cache
    II. Conflic Miss: block missed due to conflicting block placement when one restricts block placement to non-random (can lead to trash metal slowdown by a factor of 2, even 3)
    III.    Capacity Miss: working set size too big for the cache

Writes:

    I. Write-Back: update cache alongwith with a dirty bit and wait until the last minute (victim eviction time) to transfer block down to a lower level.
    II. Write-Allocate: for dealing with a write-miss, load block from lower level and update instead of updating lower level directly.  https://raw.githubusercontent.com/Adhamkmopp/cmu-comp-systems/master/Extras/ch6.jpg
