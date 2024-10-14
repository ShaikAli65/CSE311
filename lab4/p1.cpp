/*
1. Write an OpenMP program with C++ that solves the system of linear equations 𝑨𝒙 = 𝒃
using Gaussian elimination with row pivoting, followed by backward substitution.
The following components are to be shown.

(a) Write the serial version program to solves the system of linear equations, 𝑨𝒙 = 𝒃.
Calculate the execution time by using the OpenMP library function.
Test Case: N=Number of Equations, M=Number of unknowns
N=3, M=3 → x - y + z = 4, x - 4y + 2z = 8, x + 2y + 8z = 12
Solution: (x, y, z) = (53, -56, 32)
(b) Write the parallel version program to estimate the same. 
Test the result with 
(a). It includes number of threads involved and the result calculated by which thread
number. Calculate the execution time by using the OpenMP library function.
(c) Identify the line of statement which leads the Race condition. Race condition
occurs when the multiple threads accessing a shared variable. If it exists how will
you handle this problem? Use appropriate OpenMP directives/clauses and find
the solution. Test the result with value obtained in (a) and (b). Calculate the
execution time by using the OpenMP library function
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iomanip>
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl

template<typename T>
using Matrix = std::vector<std::vector<T>>;

std::string line = "\n" + std::string(31,'=') + "\n";

std::pair<Matrix<double>, std::vector<double>> test_case() {
    // N=3, M=3 → x - y + z = 4, x - 4y + 2z = 8, x + 2y + 8z = 12
    int no_of_equations = 3, no_of_unknowns = 3;
    std::vector<double> solutions = {4, 8, 12};

    Matrix<double> test_matrix = {
        {1,-1, 1,},
        {1,-4, 2,},
        {1, 2, 8,},
    };
    return {test_matrix, solutions};
}

template<typename T>
void print_matrix(const Matrix<T>& matrix) {
    std::cout << "\nmatrix:\n";
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            std::cout << std::setw(3) << element << ' ';
        }
        std::cout << '\n';
    }
}

void forwardElimination(Matrix<double> &m, std::vector<double> &results)
{
    const int &n = m.size(); 
    for (int i = 0; i < n - 1; i++) {
        // Pivoting 
        for (int k = i + 1; k < n; k++) {
            if (abs(m[i][i]) < abs(m[k][i])) {
                std::swap(m[i], m[k]);
                std::swap(results[i], results[k]);
            }
        }
        // Elimination
        for (int j = i + 1; j < n; j++) {
            double factor = m[j][i] / m[i][i];
            for (int k = i; k < n; k++) {
                m[j][k] -= factor * m[i][k];
            }
            results[j] -= factor * results[i];
        }
    }
}
void forwardEliminationParallel(Matrix<double> &m, std::vector<double> &results)
{
    const int &n = m.size(); 

    #pragma omp parallel for shared(m, results, n)
    for (int i = 0; i < n - 1; i++) {
        // Pivoting
        for (int k = i + 1; k < n; k++) {
            if (abs(m[i][i]) < abs(m[k][i])) {
                std::swap(m[i], m[k]);
                std::swap(results[i], results[k]);
            }
        }
        // Elimination
        for (int j = i + 1; j < n; j++) {
            double factor = m[j][i] / m[i][i];
            for (int k = i; k < n; k++) {
                m[j][k] -= factor * m[i][k];
            }
            results[j] -= factor * results[i];
        }
    }
}

void forwardEliminationParallelWithoutRace(Matrix<double> &m, std::vector<double> &results)
{
    const int &n = m.size(); 

    #pragma omp parallel for shared(results, n)
    for (int i = 0; i < n - 1; i++) {
        // Pivoting
        #pragma omp critical
        {
            for (int k = i + 1; k < n; k++) {
                if (abs(m[i][i]) < abs(m[k][i])) {
                    std::swap(m[i], m[k]);
                    std::swap(results[i], results[k]);
                }
            }
        }
        #pragma omp critical
        {
            for (int j = i + 1; j < n; j++) {
                double factor = m[j][i] / m[i][i];
                for (int k = i; k < n; k++) {
                    m[j][k] -= factor * m[i][k];
                }
                results[j] -= factor * results[i];
            }
        }
    }
}

std::vector<double> backWardSubstitution(Matrix<double> &m, std::vector<double> &r) {
    std::vector<double> solutions(r.size(), 0);

    for(int i = r.size() - 1; i >= 0; i--) {
        solutions[i] = r[i];
        for (size_t j = i + 1; j < r.size(); j++)
        {   
            solutions[i] -= m[i][j] * solutions[j];
        }
        solutions[i] /= m[i][i];
    }
    return solutions;
}

std::vector<double> backWardSubstitutionParallel(Matrix<double> &m, std::vector<double> &r) {
    std::vector<double> solutions(r.size(), 0);

    #pragma omp parallel for shared(m, r, solutions)
    for(int i = r.size() - 1; i >= 0; i--) {
        solutions[i] = r[i];
        for (size_t j = i + 1; j < r.size(); j++)
        {
            solutions[i] -= m[i][j] * solutions[j];
        }
        solutions[i] /= m[i][i];
    }
    return solutions;
}

std::vector<double> backWardSubstitutionParallelWithoutRace(Matrix<double> &m, std::vector<double> &r) {
    std::vector<double> solutions(r.size(), 0);

    #pragma omp parallel for shared(m, r) 
    for(int i = r.size() - 1; i >= 0; i--) {
        #pragma omp critical
        {
            solutions[i] = r[i];
            for (size_t j = i + 1; j < r.size(); j++)
            {
                solutions[i] -= m[i][j] * solutions[j];
            }
            solutions[i] /= m[i][i];
        }
    }
    return solutions;
}

void run() {
    auto [ms, rs] = test_case();
    auto s = omp_get_wtime();
    forwardElimination(ms, rs);
    auto solution = backWardSubstitution(ms, rs);
    auto e = omp_get_wtime();
    std::cout << line;
    std::cout << "Serial";
    std::cout << line << "\n";
    for (int i = 0; i < rs.size(); i++) {
        std::cout << "x" << i + 1 << " = " << std::fixed << std::setprecision(6) << solution[i] << std::endl;
    }

    std::cout << "duration " << (e - s) << "s" << std::endl;
    std::cout << line << "\n";

    auto [mp, rp] = test_case();
    
    s = omp_get_wtime();
    forwardEliminationParallel(mp, rp);
    solution = backWardSubstitutionParallel(mp, rp);
    e = omp_get_wtime();
    std::cout << line;
    std::cout << "Parallel";
    std::cout << line << "\n";
    for (int i = 0; i < rp.size(); i++) {
        std::cout << "x" << i + 1 << " = " << std::fixed << std::setprecision(6) << solution[i] << std::endl;
    }
    std::cout << "duration " << (e - s) << "s" << std::endl;
    std::cout << line << "\n";

    auto [m, r] = test_case();
    s = omp_get_wtime();
    forwardEliminationParallelWithoutRace(m,r);
    solution = backWardSubstitutionParallelWithoutRace(m, r);
    e = omp_get_wtime();
    std::cout << line;
    std::cout << "Parallel without race";
    std::cout << line << "\n";
    for (int i = 0; i < r.size(); i++) {
        std::cout << "x" << i + 1 << " = " << std::fixed << std::setprecision(6) << solution[i] << std::endl;
    }
    std::cout << "duration " << (e - s) << "s" << std::endl;
    std::cout << line << "\n";
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}