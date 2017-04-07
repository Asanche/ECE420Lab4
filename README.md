# ECE420Lab4
Repository for Lab 4

### Description
Implementing PageRank with MPI, a cluster computing library for C.

### Compilation Instructions
* Simply type make. This will make datatrim, serialtester, and main. To see the compilation commands, consult the makefile.

* To run main use mpirun -np <thread_count> ./main
* to run main on a cluster use npirun -np <thread_count> hostlist ./main