## RandomGenerators

A collection of pseudorandom number generators, each implemented in its own class. The algorithms used are:

* STDLIB - msvcrt's rand()
* LCG - Linear congruential generator
* MWC - Multiply-with-carry (CMWC)
* WELL - Well equidistributed long-period linear (WELL1024)
* MT - Mersenne twister (MT19937)

This repository also contains an example project that benchmarks them.

### Compiling

* Use <b>make</b> to compile
* This will create <i>obj</i> directory for object files and <i>bin</i> for the binary
