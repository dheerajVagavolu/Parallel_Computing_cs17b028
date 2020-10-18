
## Table of Contents


* [Algorithm](#about-the-project)
* [Built With](#built-with)
* [Usage](#usage)
* [Issues](#issues)

## Files
The assignment contains two files:

## Algorithm

### Inputs: 
> P: number of processes, S: source, D: destination 
> Read the Adjacency Matrix from the file.

### Process Division
> Divide the the adjacency matrix into squares of equal sizes and assign one to each single process.
> Create a secondary sub matrix of the size of the mini matrices to store part of the result in each process.
> take a note of the process containing the required element. ie. AdjacencyMatrix[S,D]
> Check if the element is already non-zero. In that case return 1.

### Message passing
> Pass the each processes data to each block of it's column and it's row.
> Similarily recieve data from each block of it's column and it's row.
> Calculate the submatrix from this data and store in the same process.
> The A x A is now present in parts in all the processes.
> Check if the process containing AdjacencyMatrix[S,D] != 0.
> Repeat until either iteration count exceeds number of nodes or AdjacencyMatrix[S,D] != 0. 
> Whenever AdjacencyMatrix[S,D] != 0 return the iteration count, ie. the shortest path.
> If iteration count > node count, and AdjacencyMatrix[S,D] is still equal to 0. Then no path exists between the given source and destination.

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
mpiexec -np <np> ./shortest_path <s> <d>
```
where <np> is the number of processes, <s> is the source, <d> is the destination.


