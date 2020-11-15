#include <bits/stdc++.h>
#include <iostream>
#include <omp.h>
#include <random>

using namespace std;

int main(int argc, char **argv)
{

    if (argv[2] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }

    int length = atoi(argv[2]);
    int *ar = new int[length];

    int threads = atoi(argv[1]);
    omp_set_num_threads(threads);

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

    int sum = 0;
/* Sum calculation using OpenMP reduction */
#pragma omp parallel for reduction(+ \
                                   : sum)
    for (int i = 0; i < length; i++)
        sum += ar[i];

    cout << "Array Sum: " << sum << endl;
    delete[] ar;

    return 0;
}