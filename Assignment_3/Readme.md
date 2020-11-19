## Table of Contents

- [About](#about)
- [Files](#files)
- [Usage](#usage)
- [Instruction Format](#Instructions)

## About

Parallel Computing Assignment-3

## Author

V Dheeraj (cs17b028)

## Files

- Makefile: Use this to compile the c programs.

## Usage

To remove the previous object files use:

```sh
make clean
```

To compile both the object files use:

```sh
make
```

## Instructions

Instruction format for Questions 1 to 11:

1

```sh
mpiexec -np [np] ./1 [size]
```

2

```sh
mpiexec -np [np] ./2 [size]
```

3

```sh
mpiexec -np [np] ./3 [size]
```

5

```sh
./5 [nt] [size]
```

6

```sh
./6 [threads] [size]
```

7

```sh
mpiexec -np [np] ./7 [threads] [size]
```

8

```sh
mpiexec -np [np] ./8 [threads] [size]
```

9

```sh
./9 [threads] [m] [n] [p]
```

10

```sh
mpiexec -np [np] ./10 [ar_size] [iterations]
```

11

```sh
mpiexec -np [np] ./11 [threads]
```
