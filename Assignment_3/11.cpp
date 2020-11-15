#include <bits/stdc++.h>
#include<mpi.h>
#include <omp.h>
#include <random>
#include <iostream>
#include <thread>
#include<mutex>

using namespace std;

int i=0;
int num=0;

mutex mtx; 

void iittp_barrier(int rank,int n, int tid, int threads)
{
    if(rank==0 && tid==0)
    {
        MPI_Send(&i,1,MPI_INT,1,1,MPI_COMM_WORLD);
        MPI_Recv(&i,1,MPI_INT,n-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else if(tid==0)
    {
        MPI_Recv(&i,1,MPI_INT,rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(&i,1,MPI_INT,(rank+1)%n,1,MPI_COMM_WORLD);
    }

    mtx.lock();
    num++;
    mtx.unlock();
    while((num%threads)!=0);
}


void bar_thread(int tid, int rank, int n, int threads)
{
    
    cout << "Before Barrier - Rank: " << rank <<", Thread: " << tid <<endl; 
    iittp_barrier(rank,n,tid,threads);
    cout << "After Barrier - Rank: " << rank <<", Thread: " << tid <<endl;
    
}

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    

    if (argv[1] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }
    
    int nt = atoi(argv[1]);
    omp_set_num_threads(nt);

    vector<thread> threads;
    for (int tid = 0; tid < nt; tid++)
    {
        threads.push_back(thread(bar_thread, tid, cref(rank), cref(n), cref(nt)));
    }

    for (thread &th : threads)
    {
        th.join();
    }


    MPI_Finalize();
    return 0;
}