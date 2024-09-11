/*
Write an OpenMP program with C++ that solves the system of linear equations 𝑨𝒙 =
𝒃 using LU factorization with row pivoting, followed by backward substitution. Gauss
Elimination method can be used to derive the Upper triangular matrix.
The following components are to be shown.
(a) Write the serial version program to solves the system of linear equations, 𝑨𝒙 = 𝒃.
Calculate the execution time by using the OpenMP library function.
(b) Write the parallel version program to estimate the same. Test the result with (a).
It includes number of threads involved and the result calculated by which thread
number. Calculate the execution time by using the OpenMP library function. 

*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iomanip>
#define TIME_POINT omp_get_wtime()
#define RUN_TIME(start_id, end_id) end_id - start_id
// #define TIME_POINT std::chrono::high_resolution_clock::now()
// #define RUN_TIME(start_id, end_id) (static_cast<std::chrono::duration<double>>(end_id - start_id)).count()


template<typename T>
using Matrix = std::vector<std::vector<T>>;


void run() {
    
}

// int main() {
//     //omp_set_num_threads(4);
//     auto s = TIME_POINT;
//     run();
//     auto e = TIME_POINT;
//     std::cout << "total run time: " << RUN_TIME(s, e) << std::endl;
//     return 0;
// }

#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

void luDecomposition(std::vector<std::vector<double>>& A, std::vector<int>& pivot, int n) {
    for (int i = 0; i < n; i++) {
        pivot[i] = i;  
    }

    for (int k = 0; k < n - 1; k++) {
        double max = std::abs(A[k][k]);
        int k_prime = k;
        #pragma omp parallel for shared(A, k, n) reduction(max : max, k_prime)
        for (int i = k + 1; i < n; i++) {
            if (std::abs(A[i][k]) > max) {
                max = std::abs(A[i][k]);
                k_prime = i;
            }
        }

        if (k_prime != k) {
            std::swap(A[k], A[k_prime]);
            std::swap(pivot[k], pivot[k_prime]);
        }

        #pragma omp parallel for shared(A, k, n)
        for (int i = k + 1; i < n; i++) {
            A[i][k] /= A[k][k];
            for (int j = k + 1; j < n; j++) {
                A[i][j] -= A[i][k] * A[k][j];
                std::cout << "Thread " << omp_get_thread_num << " out of " << omp_get_num_threads << " worked on row " << j << std::endl;
            }
        }
    }
}

void backSubstitution(const std::vector<std::vector<double>>& A, std::vector<double>& b, const std::vector<int>& pivot, int n) {
    std::vector<double> x(n);
    for (int i = 0; i < n; i++) {
        x[i] = b[pivot[i]];
    }

    for (int i = 1; i < n; i++) {
        #pragma omp parallel for shared(A, x, i)
        for (int j = 0; j < i; j++) {
            x[i] -= A[i][j] * x[j];
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        #pragma omp parallel for shared(A, x, i)
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }

    b = x;
}

int main() {
    int n = 3;  
    std::vector<std::vector<double>> A = {{1, -1, 1}, {1, -4, 2}, {1, 2, 8}};
    std::vector<double> b = {4, 8, 12};
    std::vector<int> pivot(n);

    double start_time = omp_get_wtime();

    luDecomposition(A, pivot, n);

    backSubstitution(A, b, pivot, n);

    double end_time = omp_get_wtime();
    
    std::cout << "Solution: ";
    for (double val : b) {
        std::cout << val << " ";
    }
    std::cout << "\nExecution Time: " << (end_time - start_time) << " seconds\n";

    return 0;
}