#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char **argv)
{

    // const char EOL = '\n';
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    int graph_size = 8;

    int root_n = sqrt(n);

    int box_size = graph_size / root_n;

    int row = (box_size * rank) / graph_size;

    int col = ((rank * box_size) % graph_size) / box_size;

    int line_start = row * box_size;
    int line_end = line_start + box_size;

    int ind_start = col * box_size;
    int ind_end = ind_start + box_size;

    printf("Rank: %d, Row: %d, Col: %d\nline start: %d, line end: %d, ind start: %d, ind end: %d\n\n", rank, row, col, line_start, line_end, ind_start, ind_end);

    // Create an array using malloc

    int *arr = (int *)malloc(box_size * box_size * sizeof(int));

    FILE *fp;
    fp = fopen("test.txt", "r");

    char *p = malloc(sizeof(char) * (graph_size));

    for (int line = 0; line < line_end; line++)
    {
        fgets(p, graph_size * 2, fp);
        // printf("%s\n", p);

        if (line >= line_start)
        {
            char test[255];
            for (int ss = ind_start; ss < ind_end; ss++)
            {

                *(arr + (line - line_start) * box_size + (ss - ind_start)) = *(p + ss) - 48;
            }
        }
    }

    for (int i = 0; i < box_size; i++)
    {
        for (int j = 0; j < box_size; j++)
        {
            printf("%d ", *(arr + i * box_size + j));
        }
        printf("\n");
    }

    // Message Passing

    free(arr);
    fclose(fp);

    // if (rank == 0)
    // {

    //     int ar[5] = {1, 2, 3, 4, 5};

    //     for (int i = 1; i < n; i++)
    //     {
    //         printf("sending data to: %d\n", i);
    //         MPI_Send(ar, 5, MPI_INT, i, i, MPI_COMM_WORLD);
    //         // x = x + 1;
    //     }

    //     // MPI_Bcast( ar , 5 , MPI_INT , 0 , MPI_COMM_WORLD);
    // }
    // else
    // {
    //     int y[5];
    //     MPI_Recv(y, 5, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
    //     printf("rank %d out of %d processors\n",
    //            world_rank, n);
    //     printf("Recieved data: %d\n", y[0]);
    // }

    // Print off a hello world message

    // Finalize the MPI environment.
    MPI_Finalize();
}