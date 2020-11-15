
## Table of Contents


* [About](#about)
* [Files](#files)
* [Algorithm](#algorithm)
* [Built With](#built-with)
* [Usage](#usage)
* [Testing](#testing)
* [Issues](#issues)

## About

* Task: Implement shortest path algorithm using matrix multiplication. Assume an undirected
graph with edge weights 1.

* Constraint1: The adjacency matrix cannot be allocated on a single process, but will occupy all the
processes on which the program is launched on

* Constraint2: The number of processes is a perfect square.

* Result: Implemented a C program to solve the given problem, (including the constraint). The output is in the format 
```c
 printf("Shortest path between source: %d and destination: %d is %d\n", source, destination, shortest_path);
```
and is shown in the command line.

## Author
V Dheeraj (cs17b028)

## Files
* Makefile: Use this to compile the c programs.
* matrix_creator.c: This is the program to generate the random adjacency matrix. It creates a file 'matrix.txt'.
* shortest_path.c: This is the core algorithm implementing the shortest path algorithm using multiple processes. It reads the matrix from 'matrix.txt'.
* While running the shortest_path.c make sure the nodes parameter matches that of the matrix.txt file.

## Algorithm

### Inputs: 
* P: number of processes, S: source, D: destination 
* Read the Adjacency Matrix from the file.

### Process Division

* Divide the the adjacency matrix into squares of equal sizes and assign one section to each single process.

![Images/process_node.png](https://github.com/dheerajrox/Parallel_Computing_cs17b028/blob/main/images/process_node.png?raw=true)

In this figure, we can see that an adjacency matrix of size 8x8 is divided into sub matrices of 4x4 to accomodate 4 processes.


* Create a secondary sub matrix of the size of the mini matrices to store part of the result in each process.
* Make a note of the process containing the required element. ie. AdjacencyMatrix[S,D]
* Check if the element is already non-zero. In that case return 1.

### Message passing
* Pass the each processes data to each block of it's column and it's row.
* Similarily recieve data from each block of it's column and it's row.
* Calculate the submatrix from this data and store in the same process.
* This is done parallely for all processes.
* The A x A is now present in pieces in all the processes.
* Check if the process containing AdjacencyMatrix[S,D] != 0.
* Repeat until either iteration count exceeds number of nodes or AdjacencyMatrix[S,D] != 0. 
* Whenever AdjacencyMatrix[S,D] != 0 return the iteration count, ie. the shortest path.
* If iteration count * node count, and AdjacencyMatrix[S,D] is still equal to 0. Then no path exists between the given source and destination.

### Built With

* [MPICH2]
* [C]

## Usage

To remove the previous object files use:
```sh
make clean
```

To compile both the object files use:
```sh
make
```

To generate a random matrix use:
```sh
./matrix_creator n 
```
where n is the number of nodes.


To run the shortest path algorithm use:
```sh
mpiexec -np <p> ./shortest_path <s> <d> <g>
```
where p is the number of processes, s is the source, d is the destination and g is the graph size (number of nodes).

* p has to be even power of 2. (1, 4, 16 ... )
* 0 <= s, d < n
* graph_size == n 

## Testing

Using the adjacency matrix: 

```sh
0100
1011
0101
0110
```


![Graph](https://github.com/dheerajrox/Parallel_Computing_cs17b028/blob/main/images/graph.png?raw=true)

The results should be: 
(Copy this adjacency matrix into matrix.txt file before running the following commands)

```sh
mpiexec -np 1 ./shortest_path 1 3 4
```
==> Shortest path between source: 1 and destination: 3 is 1

```sh
mpiexec -np 4 ./shortest_path 1 2 4
```
==> Shortest path between source: 1 and destination: 2 is 1


```sh
mpiexec -np 4 ./shortest_path 0 3 4
```
==> Shortest path between Source: 0 and Destination: 3 is 2

## Issues

* Was not sure how to stop all the processes once answer was found by a single process. Used MPI_Abort.
* In each of my process only a part of the matrix is stored, in accordance with the constraints in the assignment, but when overall space is taken into account, it is more than the actual matrix size.


