# The Malloc Lab

This is a straightforward implementation of an exclusive free list Malloc using a doubly linked list, and a last in first out (LIFO) policy for block placement. It was painful and difficult to both program and debug. The implementation itself necessitated the inclusion of an additional function to fix the list links whenever a change had occured, without which weird errors occur like links pointing to nowhere, self-referential links and by far the most common; the doubly linked list becoming circular, infinitely pointing back to itself.

This implementation works with efficiency in the 90s on all test traces except short-1 which scored 87.
