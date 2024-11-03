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
#define NUM_THREADS 7
#define CHUNK_SIZE 4
#define HEADER_LEN 25

std::vector<std::string> COLORS = {
    "\033[90m",  // Bright Black (Gray)
    "\033[91m",  // Bright Red
    "\033[92m",  // Bright Green
    "\033[93m",  // Bright Yellow
    "\033[94m",  // Bright Blue
    "\033[95m",  // Bright Magenta
    "\033[96m",  // Bright Cyan
    "\033[97m",  // Bright White
};

constexpr char BLACK[] = "\033[40m";  // Black Background
constexpr char COLOR_RESET[] = "\033[0m";

template <typename ...Args>
void print_with_id(int thread_id, Args ...args) {
    std::cout << COLOR_RESET;
    // const auto &thread_id = omp_get_thread_num();
    std::cout << COLORS[thread_id % 8] <<"[ID" << std::setw(2) << thread_id << "]" << COLOR_RESET << " ";
    (std::cout << ... << args) << std::endl;
}

std::string build_header(const std::string& prefix) {
    constexpr std::string_view newline = "\n";
    constexpr std::string_view black = BLACK;
    constexpr std::string_view reset = COLOR_RESET;

    std::string s3 = ":stats(" + prefix + "):";
    int padding_length = std::max(0, HEADER_LEN - static_cast<int>(s3.size()));

    std::string header;
    header.reserve(newline.size() + black.size() + s3.size() + padding_length + reset.size() + newline.size());

    header.append(newline)
          .append(black)
          .append(s3)
          .append(padding_length, '=')
          .append(reset)
          .append(newline);

    return header;
}

void print_stat(const std::string& prefix, const std::vector<int>& thread_loads) {
    std::cout << build_header(prefix);

    for (size_t i = 0; i < thread_loads.size(); ++i) {
        std::cout << BLACK
                  << COLORS[i % COLORS.size()] // Use modulo to wrap around color options
                  << "[THREAD#" << std::setw(2) << i << "] "
                  << "load: " << thread_loads[i] 
                  << COLOR_RESET << "\n";
    }

    std::cout << BLACK 
              << std::string(HEADER_LEN, '=') 
              << COLOR_RESET << "\n";
}

void a_serial(int n) {
    int sum = 0;
    TIME_POINT(s);
    for(int i = 1;i <= n;i++) {
        sum += i;
        const int thread_id = omp_get_thread_num();
        print_with_id(thread_id, "summing up ", sum);
    }
    TIME_POINT(e);
    RUN_TIME("serial execution total run time:", s, e);
}

void b_static(int n) {
    int sum = 0;
    std::vector<int> thread_counter;
    thread_counter.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(static) reduction(+:sum) shared(n, thread_counter)
    for(int i = 1;i <= n;i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        sum += i;
        thread_counter[thread_id] += 1;
    }
    TIME_POINT(e);
    print_stat("static", thread_counter);
    RUN_TIME("parallel static execution time :", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}

void b_static_with_chunks(int n, int chunk_count=CHUNK_SIZE) {
    int sum = 0;
    std::vector<int> thread_loads;
    thread_loads.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(static,chunk_count) shared(n) reduction(+:sum)
    for(int i = 1; i <= n; i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical 
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        thread_loads[thread_id] += 1;
        sum += i;
    }
    TIME_POINT(e);
    print_stat("static+chunk", thread_loads);
    RUN_TIME("parallel static  with chunks execution total run time:", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}

void c_dynamic(int n) {
    int sum = 0;
    std::vector<int> thread_loads;
    thread_loads.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(dynamic) reduction(+:sum) shared(n)
    for(int i = 1;i <= n;i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical 
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        thread_loads[thread_id] += 1;
        sum += i;
    }
    TIME_POINT(e);
    print_stat("dynamic", thread_loads);
    RUN_TIME("parallel dynamic execution total run time:", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}

void c_dynamic_with_chunks(int n, int chunk_count=CHUNK_SIZE) {
    int sum = 0;
    
    std::vector<int> thread_loads;
    thread_loads.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(dynamic, chunk_count) reduction(+:sum) shared(n)
    for(int i = 1;i <= n;i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical 
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        thread_loads[thread_id] += 1;
        sum += i;
    }
    TIME_POINT(e);
    print_stat("dynamic+chunk", thread_loads);
    RUN_TIME("parallel dynamic with chunks execution total run time:", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}

void d_guided(int n) {
    int sum = 0;
    
    std::vector<int> thread_loads;
    thread_loads.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(guided) reduction(+:sum) shared(n)
    for(int i = 1;i <= n;i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical 
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        thread_loads[thread_id] += 1;
        sum += i;
    }
    TIME_POINT(e);
    print_stat("guided", thread_loads);
    RUN_TIME("parallel guided execution total run time:", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}

void d_guided_with_chunks(int n, int chunk_count=CHUNK_SIZE) {
    int sum = 0;
    
    std::vector<int> thread_loads;
    thread_loads.resize(NUM_THREADS, 0);
    TIME_POINT(s);
    #pragma omp parallel for schedule(guided, chunk_count) reduction(+:sum) shared(n)
    for(int i = 1;i <= n;i++) {
        const int thread_id = omp_get_thread_num();
        #pragma omp critical 
        {
            print_with_id(thread_id, "summing up ", sum);
        }
        thread_loads[thread_id] += 1;
        sum += i;
    }
    TIME_POINT(e);
    print_stat("guided+chunk", thread_loads);
    RUN_TIME("parallel guided with chunks execution total run time:", s, e);
    std::cout << "sum of " << n << " numbers " << sum << "\n";
}


void run() {
    int n = 20;
    omp_set_num_threads(NUM_THREADS); //[PROCESS 1234] 
    a_serial(n);
    b_static(n);
    b_static_with_chunks(n);
    c_dynamic(n);
    c_dynamic_with_chunks(n);
    d_guided(n);
    d_guided_with_chunks(n);
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}