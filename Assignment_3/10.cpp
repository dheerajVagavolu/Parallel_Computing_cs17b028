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

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    int size = atoi(argv[1]);
    
    int iter = atoi(argv[2]);

    int *ar;

    for (int j = 0; j < n; j++)
    {
        if (rank == j)
        {
            ar = new int[size];

            srand(rank + time(0));
            
            for (int i = 0; i < size; i++)
            {
                ar[i] = rand()%10 + 1;
            }
            
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if(rank == 0){
            cout << "\nBefore Sending data: \n\n"; 
        }

    for (int j = 0; j < n; j++)
    {
        if (rank == j)
        {
            cout << "Process: " << rank << endl;
            for (int i = 0; i < size; i++)
            {
                cout << ar[i] << " ";
                
            }
            cout << endl;
        }
       
         MPI_Barrier(MPI_COMM_WORLD);
    }


    
    int current = 0; //r1
    int target = 0; //r2

    for(int i=0;i<iter;i++)
    {
        if(rank==current)
        {
            target=rand()%n;
            MPI_Bcast(&target,1,MPI_INT,current,MPI_COMM_WORLD);
            if(current != target){
                MPI_Send(ar,n,MPI_INT,target,1,MPI_COMM_WORLD);
            }
        }else{
            MPI_Bcast(&target, 1, MPI_INT, current,MPI_COMM_WORLD);
            if(target==rank){
                MPI_Recv(ar,n,MPI_INT,current,1,MPI_COMM_WORLD, &status);
            }
        }
        
        current=target;
        
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if(rank == 0){
            cout << "\nAfter Sending data: \n\n"; 
        }
    
    for (int j = 0; j < n; j++)
    {
        
        if (rank == j)
        {
            cout << "Process: " << rank << endl;
            for (int i = 0; i < size; i++)
            {
                cout << ar[i] << " ";
                
            }
            cout << endl;
        }
       
         MPI_Barrier(MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
    
}