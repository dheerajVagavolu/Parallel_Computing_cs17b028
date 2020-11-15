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

    int sum = 0;
    int ans = 0;
    int sub_size = size / n;

    

    int *ar;

    int extra = size % n;

    if (rank + extra >= n)
    {
        sub_size += 1;
    }



    ar = new int[size];

    srand(rank+time(0));

    for (int i = 0; i < size; i++)
    {
        // random_data.push_back(urd(re));
        // ar[i] = urd(re);
        ar[i] = rand()%100 + 1;
        // ar[i] = i;
    }


    for (int i = 0; i < n; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank)
        {
            if (rank == 0)
            {
                cout << "Before Exchange: "<<endl;
            }

            cout << "Process: " << rank << endl;

            for (int k = 0; k < size; k++)
            {
                cout << ar[k] << " ";
            }
            cout << endl;
        }
    }

    MPI_Send(ar, size, MPI_INT, (rank + 1) % n, 1, MPI_COMM_WORLD);
    MPI_Recv(ar, size, MPI_INT, (rank + n - 1) % n, 1, MPI_COMM_WORLD, &status);

    for (int i = 0; i < n; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank)
        {
            if (rank == 0)
            {
                cout << endl<<"After Exchange: "<<endl;
            }

            cout << "Process: " << rank << endl;

            for (int k = 0; k < size; k++)
            {
                cout << ar[k] << " ";
            }
            cout << endl;
        }
    }



  MPI_Finalize();
  return 0;
}