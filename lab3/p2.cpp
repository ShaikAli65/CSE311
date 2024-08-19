/*
Write an openMP program with C++ that illustrates the following OpenMP clause with its
various types.
schedule clause: It allows to specify how the iterations of the loop should be scheduled, i.e.,
allocated to threads. The various types of schedule are as follows.
(a) Write an openMP program with C++ that calculate the sum of the first N natural
numbers using for loop. (Serial Version)
(b) schedule (static), schedule (static, C) where C – number of chunks to tasks. Each chunk
contains C contiguous iterations.
(c) schedule (dynamic), schedule (dynamic, C)
(d) schedule (guided), schedule (guided, C)
(e) After completed the execution and gets verified from the faculty within submission
deadline, then do the Documentation as discussed. 
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <iomanip>
#include <cstdlib>

#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl


void a_serial(int n) {
    int sum = 0;
    for(int i = 1;i <= n;i++) {
        sum += i;
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void b_static(int n) {
    int sum = 0;
    
    #pragma omp parallel for schedule(static) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void b_static_with_chunks(int n, int chunk_count=4) {
    int sum = 0;
    
    #pragma omp parallel for schedule(static, chunk_count) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void c_dynamic(int n, int chunk_count=4) {
    int sum = 0;
    
    #pragma omp parallel for schedule(dynamic, chunk_count) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void c_dynamic_with_chunks(int n, int chunk_count=4) {
    int sum = 0;
    
    #pragma omp parallel for schedule(dynamic, chunk_count) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void d_guided(int n) {
    int sum = 0;
    
    #pragma omp parallel for schedule(guided) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}

void d_guided_with_chunks(int n, int chunk_count=4) {
    int sum = 0;
    
    #pragma omp parallel for schedule(guided, chunk_count) shared(sum, n)
    for(int i = 1;i <= n;i++) {
        #pragma omp critical 
        std::cout <<"[ID " << std::setw(2) << omp_get_thread_num() << "]"<< " summing up " << sum << "\n";     
        sum += i;
    }
    std::cout << "sum of " << n << " numbers " << sum;
}


void run() {
    int n = 20;

    omp_set_num_threads(4);
    TIME_POINT(s);
    a_serial(n);
    TIME_POINT(e);
    RUN_TIME("serial execution total run time:", s, e);
    // std::cout << "\n";
    TIME_POINT(s1);
    b_static(n);
    TIME_POINT(e1);
    RUN_TIME("parallel static execution total run time:", s1, e1);
    // std::cout << "\n";
    TIME_POINT(s2);
    b_static_with_chunks(n);
    TIME_POINT(e2);
    RUN_TIME("parallel static with chunks execution total run time:", s2, e2);
    // std::cout << "\n";
    TIME_POINT(s3);
    c_dynamic(n);
    TIME_POINT(e3);
    RUN_TIME("parallel dynamic execution total run time:", s3, e3);
    // std::cout << "\n";
    TIME_POINT(s4);
    c_dynamic_with_chunks(n);
    TIME_POINT(e4);
    RUN_TIME("parallel dynamic with chunks execution total run time:", s4, e4);
    // std::cout << "\n";
    TIME_POINT(s5);
    d_guided(n);
    TIME_POINT(e5);
    RUN_TIME("parallel guided execution total run time:", s5, e5);
    // std::cout << "\n";
    TIME_POINT(s6);
    d_guided_with_chunks(n);
    TIME_POINT(e6);
    RUN_TIME("parallel guided with chunks execution total run time:", s6, e6);
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}