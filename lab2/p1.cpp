/*
1. Write an OpenMP program with C++ that estimates the value of pi (𝜋) using a following
numerical integration formula called rectangle rule.
𝑨𝒓𝒆𝒂 = ∫
𝟒
𝟏 + 𝒙
𝟐
𝟏
𝟎
𝒅𝒙 = 𝝅
The following components are to be shown.
(a) Write the serial version program to estimate the value of pi (𝜋). Test the result with
classical integration value.
(b) Write the parallel version program to estimate the same. Test the result with classical
integration value and by (a). It includes number of threads involved and the area
calculated by which thread number.
(c) Identify the line of statement which leads the race condition. Race condition occurs when
the multiple threads accessing a shared variable. If it exists how will you handle this
problem? Use appropriate OpenMP clause and find the solution. Test the result with
classical integration value and by (a) and (b).
(d) After completed the execution and gets verified from the faculty within submission
deadline, then do the Documentation as discussed. 
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << "s" << std::endl

uint64_t n = 10'000
;

void serialPi() {
    
    long double step = 1.0 / n;
    long double sum = 0.0;
    for(uint64_t i = 0; i < n; i++) {

        long double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + (x * x));
    }

    long double pie = sum * step;
    std::cout << "PIE value : " << pie << "\n";
}

auto parallel_pi() {
    
    long double step = 1.0 / n;
    long double sum = 0.0;
    
    #pragma omp parallel for shared(sum)
    for(uint64_t i = 0; i < n; i++) {
        long double x = (i + 0.5) * step;

        #pragma omp atomic
        sum += 4.0 / (1.0 + (x * x));
    }

    long double pie = sum * step;
    std::cout << "PIE value : " << pie << "\n";
    return pie;
}
    
auto parallel_pi_using_reduction() {
    
    long double step = 1.0 / n;
    long double sum = 0.0;
    
    #pragma omp parallel for reduction(+:sum)
    for(uint64_t i = 0; i < n; i++) {
        long double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + (x * x));
    }
    std::cout << omp_get_num_threads()<< "\n";

    long double pie = sum * step;
    std::cout << "PIE value : " << pie << "\n";
    return pie;
}

void run() {
    std::cout << std::string (20,'=')<<"\n";
    TIME_POINT(s);
    serialPi();    
    TIME_POINT(e);
    RUN_TIME("serial run time: ", s, e);
    std::cout << std::string (20,'=')<<"\n";
    TIME_POINT(s1);
    parallel_pi();
    TIME_POINT(e1);
    RUN_TIME("parallel run time: ", s1, e1);
    std::cout << std::string (20,'=')<<"\n";
    TIME_POINT(s2);
    parallel_pi_using_reduction();
    TIME_POINT(e2);
    RUN_TIME("parallel reduction run time: ", s2, e2);
    std::cout << std::string (20,'=')<<"\n";
}

int main() {

    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}