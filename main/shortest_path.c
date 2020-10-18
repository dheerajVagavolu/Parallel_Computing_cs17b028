#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

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

    int graph_size = 4;

    int root_n = sqrt(n);

    int box_size = graph_size / root_n;

    int edge = (graph_size / box_size);

    int s = 3;
    int d = 2;

    int temp1_s = s / box_size;
    int temp_d = d / box_size;

    int temp2_s = s % box_size;
    int temp2_d = d % box_size;

    int required_rank = temp1_s * (edge) + temp_d;

    printf("Required Rank: s:%d d:%d box_size:%d temp1s:%d temp2_s:%d required_rank: %d\n\n\n\n\n", s, d, box_size, temp2_s, temp2_d, required_rank);

    int row = (box_size * rank) / graph_size;

    int col = ((rank * box_size) % graph_size) / box_size;

    int line_start = row * box_size;
    int line_end = line_start + box_size;

    int ind_start = col * box_size;
    int ind_end = ind_start + box_size;

    int *arr = (int *)malloc(box_size * box_size * sizeof(int));
    int *result_arr = (int *)malloc(box_size * box_size * sizeof(int));
    int *orig = (int *)malloc(box_size * box_size * sizeof(int));

    for (int i = 0; i < box_size; i++)
    {
        for (int j = 0; j < box_size; j++)
        {
            *(result_arr + i * box_size + j) = 0;
        }
    }

    FILE *fp;
    fp = fopen("test.txt", "r");

    char *p = malloc(sizeof(char) * (graph_size));

    for (int line = 0; line < line_end; line++)
    {
        fgets(p, graph_size * 2, fp);
        // printf("%s\n", p);

        if (line >= line_start)
        {

            for (int ss = ind_start; ss < ind_end; ss++)
            {

                *(arr + (line - line_start) * box_size + (ss - ind_start)) = *(p + ss) - 48;
                *(orig + (line - line_start) * box_size + (ss - ind_start)) = *(p + ss) - 48;
            }
        }
    }

    if (rank == required_rank)
    {

        int check = *(arr + temp2_s * box_size + temp2_d);
        printf("check: %d\n", check);
        if (check == 1)
        {
            printf("Shortest path: %d\n", 1);
            MPI_Abort(MPI_COMM_WORLD, 404);
        }

        // MPI_Send( const void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm);
    }

    for (int i = 0; i < box_size; i++)
    {

        for (int j = 0; j < box_size; j++)
        {

            printf("%d ", *(arr + i * box_size + j));
            // Send the info to some place and store.
        }
        printf("\n");
    }

    for (int total = 0; total < graph_size; total++)
    {

        for (int i = 0; i < box_size; i++)
        {
            for (int j = 0; j < box_size; j++)
            {
                *(result_arr + i * box_size + j) = 0;
            }
        }

        int row_low = (rank / edge) * edge;
        int row_high = row_low + edge - 1;

        int col_low = rank % edge;
        int col_high = col_low + edge * (edge - 1);

        // printf("Process: %d, row_low: %d, row_high: %d, col_low: %d, col_high: %d\n", rank, row_low, row_high, col_low, col_high);

        for (int cur_rank = row_low; cur_rank <= row_high; cur_rank++)
        {
            if (rank != cur_rank)
            {
                MPI_Send(orig, box_size * box_size, MPI_INT, cur_rank, 0, MPI_COMM_WORLD);
                // printf("Row Data sent from process %d to %d\n", rank, cur_rank);
            }
        }

        for (int cur_rank2 = col_low; cur_rank2 <= col_high; cur_rank2 += edge)
        {
            if (rank != cur_rank2)
            {

                MPI_Send(arr, box_size * box_size, MPI_INT, cur_rank2, 0, MPI_COMM_WORLD);
                // printf("Col Data sent from process %d to %d\n", rank, cur_rank2);
            }
        }

        // // Data recieving

        int req_col = col_low;

        for (int cur_rank3 = row_low; cur_rank3 <= row_high; cur_rank3++)
        {

            int *temp1 = (int *)malloc(box_size * box_size * sizeof(int));

            int *temp2 = (int *)malloc(box_size * box_size * sizeof(int));

            // printf("waiting for %d, %d\n", cur_rank3, req_col);

            if (cur_rank3 == rank)
            {
                for (int i = 0; i < box_size; i++)
                {
                    for (int j = 0; j < box_size; j++)
                    {
                        *(temp1 + i * box_size + j) = *(orig + i * box_size + j);
                    }
                }
            }
            else
            {
                MPI_Recv(temp1, box_size * box_size, MPI_INT, cur_rank3, 0, MPI_COMM_WORLD, &status);
            }

            if (req_col == rank)
            {
                for (int i = 0; i < box_size; i++)
                {
                    for (int j = 0; j < box_size; j++)
                    {
                        *(temp2 + i * box_size + j) = *(arr + i * box_size + j);
                    }
                }
            }
            else
            {
                MPI_Recv(temp2, box_size * box_size, MPI_INT, req_col, 0, MPI_COMM_WORLD, &status);
            }

            // Multiply the temp blocks and add to the result arrays
            for (int i = 0; i < box_size; i++)
            {

                for (int j = 0; j < box_size; j++)
                {
                    int ans = 0;

                    for (int k = 0; k < box_size; k++)
                    {
                        int a = *(temp1 + i * box_size + k);
                        int b = *(temp2 + k * box_size + j);
                        ans += a * b;
                    }
                    *(result_arr + i * box_size + j) += ans;
                    // Send the info to some place and store.
                }
            }

            // Copy Result array to the arr

            // printf("Data recieved from process %d and %d to %d\n", cur_rank3, req_col, rank);
            req_col += edge;
        }

        for (int i = 0; i < box_size; i++)
        {

            for (int j = 0; j < box_size; j++)
            {

                *(arr + i * box_size + j) = *(result_arr + i * box_size + j);
                // Send the info to some place and store.
            }
        }

        printf("-----------------------------------\n");

        if (rank == required_rank)
        {
            int check = *(arr + temp2_s * box_size + temp2_d);

            if (check != 0)
            {
                printf("Shortest path: %d\n", total + 2);
                MPI_Abort(MPI_COMM_WORLD, 404);
            }

            // MPI_Send( const void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm);
        }

        if (rank == 0)
        {
            for (int i = 0; i < box_size; i++)
            {

                for (int j = 0; j < box_size; j++)
                {

                    printf("%d ", *(arr + i * box_size + j));
                    // Send the info to some place and store.
                }
                printf("\n");
            }
        }
    }
    // Message Passing

    free(arr);
    free(orig);
    free(result_arr);
    fclose(fp);

    MPI_Finalize();
}