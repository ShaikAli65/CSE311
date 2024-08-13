/*
(a) Write an openMP program to print Hello World with parallel computation along with the
corresponding thread id.
*/

#include <omp.h>
#include <iostream>

int main() {
    omp_set_num_threads(4);
    #pragma omp parallel 
    {
        const int thread_id = omp_get_thread_num();
        std::cout << "Hello World from thread = " << thread_id << '\n'; 
    }
    return 0;
}