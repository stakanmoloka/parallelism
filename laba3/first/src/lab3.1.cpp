#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

void matrix_vector_product_row(double *a, double *b, double *c, int m, int n, int row_start, int row_end) {
    for (int i = row_start; i < row_end; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}

void initialize_array(double *array, int size, double value, int start, int end) {
    for (int i = start; i < end; i++) {
        array[i] = value;
    }
}

int main() {
    const int m = 20000; 
    std::vector<double> a(m * m, 0.0);  
    std::vector<double> b(m, 0.0);      
    std::vector<double> c(m, 0.0);      

    std::vector<int> num_threads = {1, 2, 4, 7, 8, 16, 20, 40};

    initialize_array(a.data(), m * m, 1.0, 0, m * m);

    initialize_array(b.data(), m, 1.0, 0, m);


    for (auto thread : num_threads) {
        int rows_per_compute_thread = m / thread;
        std::vector<std::thread> threads;

        auto start = std::chrono::steady_clock::now(); 
        for (int i = 0; i < thread; i++) {
            int start_row = i * rows_per_compute_thread;
            int end_row = (i == thread - 1) ? m : start_row + rows_per_compute_thread;
            threads.push_back(std::thread(matrix_vector_product_row, a.data(), b.data(), c.data(), m, m, start_row, end_row));
        }

        for (auto& t : threads) {
            t.join();
        }
        auto end = std::chrono::steady_clock::now(); 
        std::chrono::duration<double> time = end - start;

        std::cout << "Time for " << thread << " threads: " << time.count() << " seconds" << std::endl;
    }

    return 0;
}