/*
4. Write an openMP program (C++) for matrix multiplication with serial and parallel computation
and note the execution time of the same. Test the execution time with different dimension of
the matrix (D). For example N=10, 100, 1000…… (Any Larger Number). 
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <iomanip>


#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl

template<typename T>
using Matrix = std::vector<std::vector<T>>;

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


Matrix<int> get_random_matrix(int seed, size_t rows, size_t cols) {
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dists;

    Matrix<int> matrix;
    matrix.resize(rows, std::vector<int> (cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // matrix[i][j] = dists(gen) % 5;
            matrix[i][j] = dists(gen);
        }
    }
    return matrix;
}

template<typename T>
Matrix<T> multiply(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    Matrix<T> result;
    const uint16_t r_cols = matrix2[0].size();
    const uint16_t r_rows = matrix1.size();
    const uint16_t common_dim = matrix1[0].size();
    result.resize(r_rows, std::vector<T>(r_cols, T()));
    
    for(size_t i = 0; i < r_cols; i++) {
        for(size_t j = 0; j < r_rows; j++) {
            for(size_t k = 0; k < common_dim; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

template<typename T>
Matrix<T> pmultiply(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    Matrix<T> result;
    const uint16_t r_cols = matrix2[0].size();
    const uint16_t r_rows = matrix1.size();
    const uint16_t common_dim = matrix1[0].size();
    result.resize(r_rows, std::vector<T>(r_cols, T()));

    #pragma omp parallel for collapse(3) shared(r_cols, r_rows, common_dim)
    for(size_t i = 0; i < r_cols; i++) {
        for(size_t j = 0; j < r_rows; j++) {
            for(size_t k = 0; k < common_dim; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

void run() {
    // constexpr size_t rows = 10, cols = 10;
    // constexpr size_t rows = 100, cols = 100;
    constexpr size_t rows = 1000, cols = 1000;
    const auto&matrix1=get_random_matrix(5, rows, cols), matrix2 = get_random_matrix(6, rows, cols);
    TIME_POINT(s);
    // auto _ = multiply(matrix1, matrix2);
    auto _ = pmultiply(matrix1, matrix2);
    TIME_POINT(e);
    // print_matrix(_);
    // print_matrix(matrix1);
    // print_matrix(matrix2);
    RUN_TIME("multiplication time: ",s,e);
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}

/*
Execution times (sec):
    + --- + --------- + --------- +
    | Dim | Serial    |  Parallel |
    + --- + --------- + --------- +
    |10   | 8.6e-06   | 0.0030288 |
    + --- + --------- + --------- +
    |100  | 0.0051071 | 0.0274562 |
    + --- + --------- + --------- +
    |1000 | 5.38514   | 1.69391   |
    + --- + --------- + --------- +
*/
