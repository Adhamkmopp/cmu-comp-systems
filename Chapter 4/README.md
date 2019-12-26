## Remarks on CPUs

These are general notes on the subject of processor architecture, just enough to cover the basics of pipelining and clock signals to better fasciliate study of later chapters.

### Gates & Circuits

The levels of organization goes: Logic gates >> Combinatorial circuits (bit--level functionality) >> Aligned combinatorial circuits (word--level functionality; multiplexors). One important combinatorial circuit is the arithmetic logic unit which can perform additions, subtractions, bitwise AND and NOT operations based on a 2--bit control signal. Combinatorial circuits respond continuously to changes in input after some delay, which acts as an essential factor in determing clock signal frequency (length of each clock cycle).

### Clocks & Signals

Not concidered a combinatorial circuit, register files can perform writes and reads according to a destination or source input along with an output port or a value input port, all regulated by a clock signal. The clock signal itself rises and falls each cycle, signaling a new step forward in processing. The same design applies to random access memories.

Other parts of the CPU may or may not be control by a clock signal. Combinatorial circuits do not appear to be regulated by an extrenal clock. Literal word--sized bundles of wires run across the CPU to carry full words from one part to the next (64 on a modern CPU). The main gist of it is, delays as combinatorial circuits respond to new inputs, or in updating register/memory values determines the length of clock cycles. In pipelined CPUs (virtually all modern CPUs), the matter is further complicated by breaking down instructions into discrete steps and allowing the instructions to, for lack of a better term, chase one another in sequence as they proceed through different parts. This in theory increases efficiency by packing as many simultanously executing instructions into one clock cycle as possible, but it does increase latency; disaster can ensue if the clock cycle is less than some delay bottleneck in the process, effectively letting one step to overwrite another.
