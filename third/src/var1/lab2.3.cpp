#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <fstream>


int N = 4000;
int iterations = 5000;


double sum_of_vector(const std::vector<double>& x_new, const std::vector<double>& x, int thread) {
    double sum = 0.0;
    #pragma omp parallel for schedule(guided, 20) reduction(+:sum) num_threads(thread)
    for (size_t i = 0; i < x_new.size(); i++) {
        sum += pow(x_new[i] - x[i], 2);
    }
    return sqrt(sum);
}

std::vector<double> simple_iteration(const std::vector<std::vector<double>>& A, const std::vector<double>& b, int thread) {
    std::vector<double> x(N, 0.0);
    std::vector<double> x_new(N, 0.0);
    
    for (int iteration = 0; iteration < iterations; iteration++) {
        #pragma omp parallel for schedule(guided, 20) num_threads(thread)
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                sum += A[i][j] * x[j];
            }
            x_new[i] = x[i] - 0.00001 * (sum - b[i]);
        }

        if (sum_of_vector(x_new, x, thread) < 1e-6) {
            std::cout<<"could find answer for "<<iteration+1<<" iterations"<<std::endl;
            std::cout<<"answer: "<<x_new[0]<<std::endl;
            return x_new;
        }

        x = x_new;
    }
    

    std::cout << "its hasnt answer after"<<iterations<< " iterations"<<std::endl;
    std::cout<<"answer: "<<x_new[0]<<std::endl;
    return x_new;
}

int main(){
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 1.0));
    std::vector<double> b(N, N + 1.0);
    std::ofstream file("var2.csv");

    for (int i=0;i<N;i++){
        A[i][i]=2;
    }

    auto start = std::chrono::steady_clock::now(); 
    std::vector<double> res = simple_iteration(A, b, 1);
    auto end = std::chrono::steady_clock::now(); 
    std::chrono::duration<double> time_1 = end - start;
    std::cout<<"time: "<<time_1.count()<< " for "<< 1<<" threads" << std::endl;
    file << time_1.count() << "\n";

    int threads = omp_get_num_procs();
    for (int thread = 2; thread <= threads; thread++){
        auto start = std::chrono::steady_clock::now(); 
        std::vector<double> res = simple_iteration(A, b, thread);
        auto end = std::chrono::steady_clock::now(); 
        std::chrono::duration<double> time = end - start;
        std::cout<<"time: "<<time.count()<< "for "<< thread<<" threads" <<"S: "<<time_1.count()/time.count()<< std::endl;
        file << time.count() << "\n";
    }
}

