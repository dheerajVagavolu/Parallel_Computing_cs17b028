#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void create_adjacency_matrix(int n);

// Pass n as command line argument

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    create_adjacency_matrix(n);
    return 0;
}

void create_adjacency_matrix(int n)
{
    FILE *fp;
    fp = fopen("matrix.txt", "w+");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(fp, "%d", rand() % 2);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}