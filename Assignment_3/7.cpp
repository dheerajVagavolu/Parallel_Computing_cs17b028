#include <bits/stdc++.h>
#include <iostream>
#include <omp.h>
#include <random>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argv[2] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }

    MPI_Init(NULL, NULL);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    int size;

    int threads = atoi(argv[1]);
    omp_set_num_threads(threads);

    int length = atoi(argv[2]);

    int sub_size = length / n;

    int extra = length % n;

    int *ar;

    if (rank + extra >= n)
    {

        sub_size += 1;
    }

    int sum = 0;
    int ans = 0;

    if (rank == 0)
    {
        ar = new int[length];

        double lbound = 0;
        double ubound = 10;
        uniform_real_distribution<double> urd(lbound, ubound);
        default_random_engine re;

        for (int i = 0; i < length; i++)
        {
            // random_data.push_back(urd(re));
            ar[i] = urd(re);
            // ar[i] = i;
        }

        for (int i = 1; i < n; i++)
        {
            int begin, end;
            int send_size;
            if (i + extra >= n)
            {

                begin = length - ((sub_size+1) * (n - i));
                end = length - ((sub_size+1) * (n - i - 1));
                send_size = sub_size+1;
            }
            else
            {
                begin = sub_size * i;
                end = sub_size * (i + 1);
                send_size = sub_size;
            }

            MPI_Send(ar + begin, send_size, MPI_INT, i, 1, MPI_COMM_WORLD);

    }
    #pragma omp parallel for reduction(+: sum) 
        for (int i = 0; i < sub_size; i++)
        {
            sum += ar[i];
        }
        
    MPI_Reduce(&sum, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        delete[] ar;

        cout << "Array Sum: " << ans << endl;
}
else
{
    ar = new int[sub_size];
    MPI_Recv(ar, sub_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
#pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < sub_size; i++)
    {
        sum += ar[i];
    }

    MPI_Reduce(&sum, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    delete[] ar;
}

MPI_Finalize();
return 0;
}