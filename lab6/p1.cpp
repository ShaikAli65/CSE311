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
// #define PARALLEL

#include <bits/stdc++.h>
#include <vector>
#include <omp.h>


int main()
{
    auto start_time = TIME_POINT;
    int n = 3;
    // std::cout << "Enter the number of equations : ";
    // std::cin >> n;
    // std::vector<std::vector<double>> A(n, std::vector<double>(n));
    // std::vector<double> B(n);

    std::vector<std::vector<double>> A = {
        {1,2,3},
        {3,1,4},
        {5,3,1},
    };

    std::vector<double> B = {
        14,17,14
    };

    // std::cout << "Fill matrix A : " << std::endl;
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {
    //         std::cin >> A[i][j];
    //     }
    // }
    // std::cout << "---------------------------\n";
    // std::cout << "Enter matrix B : " << std::endl;
    // for (int i = 0; i < n; i++)
    // {
    //     std::cin >> B[i];
    // }
    
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    std::vector<std::vector<double>> U(n, std::vector<double>(n));

    // Decomposing matrix into L and U
    for (int i = 0; i < n; i++)
    {
        // Fill upper triangular matrix U
#ifdef PARALLEL
        #pragma omp parallel for
#endif
        for (int j = i; j < n; j++)
        {
            U[i][j] = A[i][j];
        }
        // Fill lower triangular matrix L and update A
#ifdef PARALLEL
        #pragma omp parallel for
#endif
        for (int j = i + 1; j < n; j++)
        {
            L[j][i] = A[j][i] / A[i][i]; // Factor
            for (int k = i; k < n; k++)
            {
                A[j][k] -= L[j][i] * U[i][k];
            }
            // #pragma omp critical
            // std::cout<<"thread - "<<omp_get_thread_num()<<std::endl;
        }
    }
    
    // Set diagonal elements of L to 1
    for (int i = 0; i < n; i++)
    {
        L[i][i] = 1.0;
    }
    
    // Display L matrix
    std::cout << "---------------------------\n";
    std::cout << "L matrix: " << std::endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << L[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // Display U matrix
    std::cout << "---------------------------\n";
    std::cout << "U matrix: " << std::endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << U[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // Forward substitution to solve LY = B
    std::vector<double> y(n);
    for (int i = 0; i < n; i++)
    {
        y[i] = B[i];
#ifdef PARALLEL
        #pragma omp parallel for
#endif
        for (int j = 0; j < i; j++)
        {
#ifdef PARALLEL
            #pragma omp atomic
#endif
            y[i] -= L[i][j] * y[j];
        }
    }
    std::cout << "---------------------------\n";
    std::cout << "Y matrix: " << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << y[i] << std::endl;
    }

    // Backward substitution to solve UX = Y
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = y[i];

#ifdef PARALLEL
        #pragma omp parallel for
#endif
        for (int j = i + 1; j < n; j++)
        {
#ifdef PARALLEL
            #pragma omp atomic
#endif            
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
    
    std::cout << "---------------------------\n";
    std::cout << "X matrix (solution): " << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << x[i] << std::endl;
    }
    std::cout << "---------------------------\n";

    auto end_time = TIME_POINT;
    auto elapsed_time = RUN_TIME(start_time, end_time);
    // elapsed_time = elapsed_time * 1e3;
    std::cout << "Execution time: " << elapsed_time << " milli seconds" << std::endl;
    return 0;
}
