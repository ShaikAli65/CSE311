/*
b) Write an openMP program to print number of threads.

*/

#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()

void run() {
    std::cout << "thread count : " << omp_get_num_threads() << '\n';
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    // std::cout << "\033[92m" << "asdasd";
    TIME_POINT(e);
    std::chrono::duration<double> d = e - s;
    std::cout << "\nrun time: " << d.count() << "s\n";
    return 0;
}