#include <bits/stdc++.h>
// #include <iostream>
// #include <thread>
// #include <vector>
// #include <random>

using namespace std;

int n;
int length;
int extra;
int sub_size;
int *sum;

void local_sum(int id, const vector<int> &data)
{

    int begin, end;

    if (id + extra >= n)
    {

        begin = length - ((sub_size + 1) * (n - id));
        end = length - ((sub_size + 1) * (n - id - 1));
    }
    else
    {
        begin = sub_size * id;
        end = sub_size * (id + 1);
    }

    for (int i = begin; i < end; i++)
    {
        sum[id] += data[i];
    }
}

int main(int argc, char **argv)
{
    if (argv[2] == NULL)
    {
        cout << "Provide all the values. Check readme for the correct format." << endl;
        exit(0);
    }

    n = atoi(argv[1]);
    length = atoi(argv[2]);

    sub_size = length / n;
    extra = length % n;

    // Array of threads
    sum = new int[n];

    double lbound = 0;
    double ubound = 10;
    uniform_real_distribution<double> urd(lbound, ubound);
    default_random_engine re;
    vector<int> random_data;
    for (int i = 0; i < length; i++)
    {
        random_data.push_back(urd(re));
        // random_data.push_back(i);
    }

    vector<thread> threads;
    for (int i = 0; i < n; i++)
    {
        threads.push_back(thread(local_sum, i, cref(random_data)));
    }

    for (thread &th : threads)
    {
        th.join();
    }

    int total = 0;
    for (int i = 0; i < n; i++)
    {
        total += sum[i];
    }

    cout << "Array sum: " << total << endl;

    delete[] sum;
    return 0;
}