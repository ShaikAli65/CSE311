/*
5. Write an openMP program (C++) for the following sorting algorithms with serial and parallel
computation and note the execution time of the same. Analyse the output with different high
N values, where N is the number of elements to be sorted. (Like above table)
a) Quick sort 
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl


void run() {
    
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}