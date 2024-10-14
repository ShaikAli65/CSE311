/*
3. Write an openMP to print “Hello World” for k times using for loop and compute the execution
time of sequential and parallel run.
*/

#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl


void seq_print() {
    for(int i = 0; i < 100; i++) {
        std::cout << "hello world\n";
    }

}

void parallel_print() {
    #pragma omp parallel for
    for(int i = 0; i < 100; i++) {
        std::cout << "hello world" << omp_get_thread_num() << "\n";
    }
}

void run() {
    TIME_POINT(sequential_start);
    seq_print();
    TIME_POINT(sequential_end);
    parallel_print();
    TIME_POINT(parallel_end);
    RUN_TIME( "sequential run time: ", sequential_start, sequential_end);
    RUN_TIME( "parallel run time: ", sequential_end, parallel_end);
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}