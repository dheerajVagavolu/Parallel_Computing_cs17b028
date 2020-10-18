// Get the number of processes n from the input

// Create a random adjacency matrix of root(n) x root(n)

// Assign first n processes to

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void create_adjacency_matrix(int n);

int main(int argc, char **argv)
{

    int n = atoi(argv[1]);
    create_adjacency_matrix(n);

    // char buff[255];

    // fprintf(fp, "This is testing 1 ... \n");
    // fprintf(fp, "This is testing 2 ... \n");
    // fprintf(fp, "This is testing 3 ... \n");
    // fputs("This is testing 4 ... \n", fp);
    // fgets(buff, 255, (FILE *)fp);
    // printf("%s\n", buff);

    return 0;
}

void create_adjacency_matrix(int n)
{
    FILE *fp;
    fp = fopen("test.txt", "w+");

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