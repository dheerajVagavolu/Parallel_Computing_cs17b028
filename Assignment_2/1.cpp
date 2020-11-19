#include <bits/stdc++.h>
#include <iostream>
#include <omp.h>
#include <random>
#include <chrono>
#include<mutex>
#include <fstream>

using namespace std;

mutex mtx; 
int check = 0;
int nthreads;

void thread_barrier(int threads){
    mtx.lock();
    check++;
    mtx.unlock();
    while((check%threads)!=0);
}

int main(int argc, char **argv){

    int max_threads = omp_get_max_threads();
    cout << "\n(Maximum number of threads: " << max_threads << ")" << endl;
    
    
    if(argv[2] == NULL){
        cout << "Provide all the values. Check readme for the correct format." << endl;
        return -1;
    }

    if(argv[3]!=NULL){
        int temp = atoi(argv[3]);
        omp_set_num_threads(temp);
        nthreads = temp;
    }else{
        
        cout << "Number of threads not provided as argument, launching on "<< max_threads <<" threads" << endl;
        omp_set_num_threads(max_threads);
        nthreads = max_threads;
    }

    double th = atof(argv[2]);
    
    int size = atoi(argv[1]);
    
            
            // Initializing the Matrix

    double lbound = 0.0;
    double ubound = 10.0;
    uniform_real_distribution<double> urd(lbound, ubound);
    default_random_engine re;

    double** mat = new double*[size];
    for(int i = 0; i < size; ++i){
        mat[i] = new double[size];
    }

    double** mat_old = new double*[size];
    for(int i = 0; i < size; ++i){
        mat_old[i] = new double[size];
        for(int j = 0; j<size;j++){
            mat[i][j] = urd(re);
            mat_old[i][j] = mat[i][j];
        }
    }

    double *tids = new double[nthreads];

    cout << "Inputs: n = " << size << ", threshold = " << th << endl;

    int run = 1;
    int iter = 0;

    auto start = chrono::high_resolution_clock::now();


    #pragma omp parallel 
    {
            int tid = omp_get_thread_num();
            double local_max = 0;

            // int nthreads = omp_get_num_threads();
            
        while(run == 1)
        {
            int local_run = 1;
            #pragma omp for schedule(static, size/nthreads)
            for(int i = 0; i < size; i++){
                for(int j = 0; j<size; j++){
                    double temp = 0;
                    
                    if (i > 0 && i < size-1){
                        if( j > 0 && j < size - 1){
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][(j+1)] \
                                + mat_old[(i+1)][(j)] + mat_old[(i)][(j-1)]) / 5;
                        }else if(j == 0){
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][(j+1)] \
                                + mat_old[(i+1)][(j)]) / 4;
                        }else{
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][0] \
                                + mat_old[(i+1)][(j)]) / 4;
                        }
                    }else if(i == 0){
                        if( j > 0 && j < size - 1){
                            temp += (mat_old[i][j] + mat_old[(i)][(j+1)] \
                                + mat_old[(i+1)][(j)] + mat_old[(i)][(j-1)]) / 4;
                        }else if(j == 0){
                            temp += (mat_old[i][j] + mat_old[(i)][(j+1)] \
                                + mat_old[(i+1)][(j)]) / 3;
                        }else{
                            temp += (mat_old[i][j] + mat_old[(i)][0] \
                                + mat_old[(i+1)][(j)]) / 3;
                        }
                    }else{
                        if( j > 0 && j < size - 1){
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][(j+1)] \
                                + mat_old[(i)][(j-1)]) / 4;
                        }else if(j == 0){
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][(j+1)])/ 3;
                        }else{
                            temp += (mat_old[i][j] + mat_old[(i-1)][(j)] + mat_old[(i)][0]) / 3;
                        }
                    }

                    mat[i][j] = temp;
                    double diff = mat[i][j] - mat_old[i][j];
                    if(diff > local_max){
                        local_max = diff;
                    }
                }
            }

            tids[tid] = local_max; 

            // cout << "Thread id: " << tid << " Started" << endl;

            thread_barrier(nthreads);

            if(tid == 0){
                double total_max = 0;
                for(int i = 0; i < nthreads; i++){
                    if(tids[i] > total_max){
                        total_max = tids[i];
                    }
                }

                // cout << "Total Max: " << total_max << endl;
                iter += 1;
                if(total_max < th){
                    run = 0;
                    // cout << "RAN " << run << endl;
                    // break;
                }
            }


            // cout << "End 2: " << run << endl;

            // #pragma omp atomic read
            // local_run = run;
            // cout << "Local_run" << local_run << endl;
            
            
            // if(run == 0){
            //     cout << "Hope" << endl;
            //     break;
            // }


            local_max = 0;
            // cout << "Thread id: " << tid << " Started" << endl;
            
            // thread_barrier(nthreads);
            // Copy the new values to the old array

            #pragma omp for schedule(static, size/nthreads)
            for(int i = 0; i < size; i++){
                for(int j =0; j < size; j++){
                    mat_old[i][j] = mat[i][j];
                }
            }
            

            // thread_barrier(nthreads);
            // cout << "Thread id: " << tid << " Completed" << endl;
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout <<"\nTime taken: "<< duration.count() <<  "ms, Iterations: " << iter << ", Number of threads launched: "<< nthreads<<  endl << endl;

return 0;

}