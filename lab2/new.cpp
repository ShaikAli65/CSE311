#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel 
    {
        // omp_
        std::cout << "Hello World from thread = %d of %d\n" << omp_get_thread_num() << '\n'; 
    }
    return 0;
}