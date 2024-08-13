#include <omp.h>
#include <iostream>
#include <chrono>

int main() {
    omp_set_num_threads(4);
    int i= 0;
    auto s = std::chrono::high_resolution_clock::now();
    #pragma omp parallel 
    {
        
        std::cout << omp_get_thread_num() << std::endl;
        // std::cout << omp_get_num_threads() << '\n';
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = e - s;

    return 0;
}

