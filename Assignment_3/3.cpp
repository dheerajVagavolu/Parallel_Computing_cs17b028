#include <bits/stdc++.h>
#include <iostream>
#include <omp.h>
#include <random>
#include <mpi.h>

using namespace std;

int n;
int length;
int extra;
int sub_size;
int *sum;


int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    int size;

    if (argv[1] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }else{
        size = atoi(argv[1]);
    }


    int sub_size = size / n;

    int extra = size % n;

    int begin, end;

    if (rank + extra >= n)
    {
        sub_size += 1;
        begin = size - (sub_size * (n - rank));
        end = size - (sub_size * (n - rank - 1));
    }
    else
    {
        begin = sub_size * rank;
        end = sub_size * (rank + 1);
    }

    int *ar;

    ar = new int[sub_size];

    for (int i = begin; i < end; i++)
    {
        ar[i - begin] = i;
    }

    int ans = 0;
    int sum = 0;

    for (int i = 0; i < sub_size; i++)
    {
        sum += ar[i];
    }

    MPI_Reduce(&sum, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    delete[] ar;

if (rank == 0)
    {
        cout << "Array Sum: " << ans << endl;
    }

    MPI_Finalize();
    return 0;


    

}