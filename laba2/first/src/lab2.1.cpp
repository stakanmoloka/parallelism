#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>

void matrix_vector_product_omp(double *a, double *b, double *c, int m, int n, int threads)
{
    #pragma omp parallel num_threads(threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
            for (int i = lb; i <= ub; i++) {
                c[i] = 0.0;
                for (int j = 0; j < n; j++)
                c[i] += a[i * n + j] * b[j];

            }
    }
}

void matrix_vector_product(double *a, double *b, double *c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

int main() {
    double *a, *b, *c;
    int threads[] = {1, 2, 4, 7, 8 ,16, 20, 40};
    int m = 20000, n = 20000;
    a = (double*)malloc(sizeof(*a) * m * n);
    b = (double*)malloc(sizeof(*b) * n);
    c = (double*)malloc(sizeof(*c) * m);    
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++) {
            for (int j = 0; j < n; j++)
                a[i * n + j] = i + j;
                c[i] = 0.0;
        }
    }
    for (int j = 0; j < n; j++)
    b[j] = j;
    auto start_serial = std::chrono::steady_clock::now(); 
    matrix_vector_product(a, b, c, m, n);       
    auto end_serial = std::chrono::steady_clock::now(); 
    std::chrono::duration<double> serial_time = end_serial - start_serial;

    for (int thread : threads){
        auto start = std::chrono::steady_clock::now(); 
        matrix_vector_product_omp(a, b, c, m, n, thread);       
        auto end = std::chrono::steady_clock::now();    
        std::chrono::duration<double> omp_time = end - start;
        
        std::cout << "Elapsed time for " << thread << " threads: " << omp_time.count() << " sec. " << "S = "<<serial_time.count() /omp_time.count()  <<" m = n = "<<m << std::endl;
    }

    free(a);
    free(b);
    free(c);

    m = 40000, n = 40000;
    a = (double*)malloc(sizeof(*a) * m * n);
    b = (double*)malloc(sizeof(*b) * n);
    c = (double*)malloc(sizeof(*c) * m);    
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++) {
            for (int j = 0; j < n; j++)
                a[i * n + j] = i + j;
                c[i] = 0.0;
        }
    }
    for (int j = 0; j < n; j++)
    b[j] = j;
    start_serial = std::chrono::steady_clock::now(); 
    matrix_vector_product(a, b, c, m, n);       
    end_serial = std::chrono::steady_clock::now(); 
    serial_time = end_serial - start_serial;

    for (int thread : threads){
        auto start = std::chrono::steady_clock::now(); 
        matrix_vector_product_omp(a, b, c, m, n, thread);       
        auto end = std::chrono::steady_clock::now();    
        std::chrono::duration<double> omp_time = end - start;
        
        std::cout << "Elapsed time for " << thread << " threads: " << omp_time.count() << " sec. " << "S = "<<serial_time.count() /omp_time.count()  <<" m = n = "<<m << std::endl;
    }
    return 0;
}
