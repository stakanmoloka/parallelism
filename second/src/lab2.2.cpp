#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <cmath>



double func(double x)
{
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), double a, double b, int n, int threads)
{
    double h = (b - a) / n;
    double sum = 0.0;
    #pragma omp parallel num_threads(threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);
        double sumloc = 0.0;
        for (int i = lb; i <= ub; i++)
            sumloc += func(a + h * (i + 0.5));
        #pragma omp atomic
        sum += sumloc;
    }
    sum *= h;
    return sum;
}

double integrate(double (*func)(double), double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += func(a + h * (i + 0.5));
    sum *= h;
    return sum;
}


const double a = -4.0;
const double b = 4.0;
const int nsteps = 40000000;


int main() {
    int threads[] = {1, 2, 4, 7, 8 ,16, 20, 40};
    auto start_serial = std::chrono::steady_clock::now(); 
    double res = integrate(func, a, b, nsteps);    
    auto end_serial = std::chrono::steady_clock::now(); 
    std::cout<<"Result (serial):"<<res<<std::endl;
    std::chrono::duration<double> serial_time = end_serial - start_serial;

    for (int thread : threads){
        auto start = std::chrono::steady_clock::now(); 
        res = integrate_omp(func, a, b, nsteps, thread);    
        auto end = std::chrono::steady_clock::now();    
    
        std::chrono::duration<double> omp_time = end - start;
        std::cout<<"Result (parallel): "<<res<< " time for " << thread << " threads: " << omp_time.count() << " sec. " << "S = "<<serial_time.count() /omp_time.count()<< std::endl;
    }

    return 0;
}
