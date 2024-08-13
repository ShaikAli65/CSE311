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

#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl

template<typename T>
using Matrix = std::vector<std::vector<T>>;

void readMatrix(Matrix<int>& matrix, int rows, int cols) {
    matrix.resize(rows, std::vector<int>(cols));
    std::cout << "Enter the matrix elements row by row:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cin >> matrix[i][j];
        }
    }
}

template<typename T>
void printMatrix(const typename Matrix<T>::Type& matrix) {
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            std::cout << element << ' ';
        }
        std::cout << '\n';
    }
}   

std::pair<Matrix<int>, Matrix<int>> get_input() {
    int rows1, cols1, rows2, cols2;
    std::cout << "Enter the dimensions of the first matrix (rows cols): ";
    std::cin >> rows1 >> cols1;
    std::vector<std::vector<int>> matrix1;
    readMatrix(matrix1, rows1, cols1);
    std::cout << "Enter the dimensions of the second matrix (rows cols): ";
    std::cin >> rows2 >> cols2;
    std::vector<std::vector<int>> matrix2;
    readMatrix(matrix2, rows2, cols2); 
    return {matrix1, matrix2};
}

template<typename T>
Matrix<T> multiply(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    Matrix<T> result;
    const uint16_t r_cols = matrix2[0].size();
    const uint16_t r_rows = matrix1.size();
    const uint16_t common_dim = matrix1[0].size();
    result.resize(r_rows, std::vector<T>(r_cols, T()));

    for(uint16_t i = 0; i < r_cols; i++) {
        for(uint16_t j = 0; j < r_rows; j++) {
            for(uint16_t k = 0; k < common_dim; k++) {
                result[i][j] = matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

void run() {
    const auto& [matrix1, matrix2] = get_input();
    TIME_POINT(s);
    auto _ = multiply(matrix1, matrix2);
    TIME_POINT(e);
    RUN_TIME("totol time: ",s,e);
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}