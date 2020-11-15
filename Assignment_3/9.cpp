#include <bits/stdc++.h>
#include <iostream>
#include <omp.h>
#include <random>
#include <mpi.h>


using namespace std;

int main(int argc, char *argv[])
{
    if (argv[4] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }
    
    int threads = atoi(argv[1]);
    omp_set_num_threads(threads);

    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int p = atoi(argv[4]);
    
    
    int** a = new int*[m];
    for(int i = 0; i < m; ++i)
        a[i] = new int[n];

    int** b = new int*[n];
    for(int i = 0; i < n; ++i)
        b[i] = new int[p];
    
    int** ans = new int*[m];
    for(int i = 0; i < m; ++i)
        ans[i] = new int[p];
    
    

    double lbound = 0;
    double ubound = 10;
    uniform_real_distribution<double> urd(lbound, ubound);
    default_random_engine re;
    int sub_size = m / threads;    


    #pragma omp parallel 
    {

        int id = omp_get_thread_num();

        /* Initialize matrices randomly */
        #pragma omp for schedule(static, sub_size)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                a[i][j] = urd(re);
        #pragma omp for schedule(static, sub_size)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < p; j++)
                b[i][j] = urd(re);
        #pragma omp for schedule(static, sub_size)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < p; j++)
                ans[i][j] = 0;


        #pragma omp for schedule(static, sub_size)
        for (int i = 0; i < m; i++)
        {
            
            for (int j = 0; j < p; j++)
                for (int k = 0; k < n; k++)
                    ans[i][j] += a[i][k] * b[k][j];
        }
    }

    
    
    cout << "Produt: " << endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("%d   ", ans[i][j]);
        cout << endl;
    }
    cout << endl;

    return 0;
}