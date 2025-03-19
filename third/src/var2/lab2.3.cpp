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
int M = N; // Количество столбцов (для квадратной матрицы)

double sum_of_vector(const std::vector<double>& x_new, const std::vector<double>& x, int thread) {
    double sum = 0.0;

    #pragma omp parallel num_threads(thread)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();

        int items_per_thread = N / nthreads;

        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (N - 1) : (lb + items_per_thread - 1);

        double local_sum = 0.0;
        for (int i = lb; i <= ub; i++) {
            local_sum += pow(x_new[i] - x[i], 2);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    return sqrt(sum);
}

// Изменение сигнатуры функции, чтобы она работала по ссылке
std::vector<double>& simple_iteration(std::vector<double>& A, const std::vector<double>& b, int thread) {
    std::vector<double> x(N, 0.0);
    std::vector<double> x_new(N, 0.0);

    for (int iteration = 0; iteration < iterations; iteration++) {
        #pragma omp parallel num_threads(thread)
        {
            int nthreads = omp_get_num_threads();
            int threadid = omp_get_thread_num();

            int items_per_thread = N / nthreads;

            int lb = threadid * items_per_thread;
            int ub = (threadid == nthreads - 1) ? (N - 1) : (lb + items_per_thread - 1);

            for (int i = lb; i <= ub; i++) {
                double sum = 0.0;
                for (int j = 0; j < N; j++) {
                    sum += A[i * M + j] * x[j]; // Обращение к элементу матрицы
                }
                x_new[i] = x[i] - 0.00001 * (sum - b[i]);
            }
        }

        if (sum_of_vector(x_new, x, thread) < 1e-6) {
            std::cout << "could find answer for " << iteration + 1 << " iterations" << std::endl;
            std::cout << "answer: " << x_new[0] << std::endl;
            x = x_new;
            return x_new;
        }

        x = x_new;
    }

    std::cout << "its hasn't answer after " << iterations << " iterations" << std::endl;
    std::cout << "answer: " << x_new[0] << std::endl;
    return x_new;
}

int main() {
    std::vector<double> A(N * M, 1.0); // Одномерный вектор для матрицы
    std::vector<double> b(N, N + 1.0);
    std::ofstream file("var2.csv");

    for (int i = 0; i < N; i++) {
        A[i * M + i] = 2.0; // Диагональные элементы
    }

    auto start = std::chrono::steady_clock::now();
    std::vector<double>& res = simple_iteration(A, b, 1);  // Работает со ссылкой
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_1 = end - start;
    std::cout << "time: " << time_1.count() << " for " << 1 << " threads" << std::endl;
    file << time_1.count() << "\n";

    int threads = omp_get_num_procs();
    for (int thread = 2; thread <= threads; thread++) {
        auto start = std::chrono::steady_clock::now();
        std::vector<double>& res = simple_iteration(A, b, thread);  // Работает со ссылкой
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> time = end - start;
        std::cout << "time: " << time.count() << " for " << thread << " threads " << "S: " << time_1.count() / time.count() << std::endl;
        file << time.count() << "\n";
    }
}
