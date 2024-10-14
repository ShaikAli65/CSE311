/*
Write the OpenMP program with C/C++ with the desired output for the following
problem. A prime number is a positive integer evenly divisible by exactly two positive
integers: itself and 1. Take first five prime numbers are 2, 3, 5, 7, and 11. Sometimes
two consecutive odd numbers are both prime. For example, the odd integers following
3, 5, and 11 are all prime numbers. However, the odd integer following 7 is not a prime
number.
a) Write a parallel program to determine, for all integers less than 200, the number
of times that two consecutive odd integers are both prime.
b) On the same program compute the sum of each two consecutive odd integers are
both prime, for all integers less than 200. How to handle the race condition in this
problem with the help of OpenMP directives/clauses?
-------------------------------------------------------------------------------------------------
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iomanip>
#define TIME_POINT omp_get_wtime()
#define RUN_TIME(start_id, end_id) end_id - start_id
// #define TIME_POINT std::chrono::high_resolution_clock::now()
// #define RUN_TIME(start_id, end_id) (static_cast<std::chrono::duration<double>>(end_id - start_id)).count()

bool isprime(const int &in) {
    for (size_t i = 2; i <= sqrt(in); i++)
    {
        if (in % i == 0)
            return false;
    }
    return true;
}

void run(const int &count) {
    std::vector<int> prime_numbers;
    for (int i = 3; i < count; i += 2)
    {
        if(isprime(i))
            prime_numbers.push_back(i);
    }
    int c = 0;
    int prev_prime = 3;
    std::unordered_set<int> elements;
    for (size_t i = 0; i < prime_numbers.size(); i++)
    {
        if (prime_numbers[i] == prev_prime + 2)
        {
            c++;
            elements.emplace_hint(std::begin(elements), prev_prime);
            elements.emplace_hint(std::begin(elements), prev_prime + 2);
            // std::cout << prev_prime << " " << prev_prime + 2 << "\n"; 
        }
        prev_prime = prime_numbers[i];
    }
    std::cout << "count: "<< c << "\nsum : ";
    std::cout << std::accumulate(elements.begin(), elements.end(), 0);
    std::cout << "\n";
}

void parallel_run(const int &count) {
    std::unordered_set<int> odd_prime_number_pairs;
    int no_of_pairs = 0;
    
    #pragma omp parallel for shared(odd_prime_number_pairs)
    for (int i = 3; i < count; i += 2)
    {
        if(isprime(i) && isprime(i + 2))
        {
            #pragma omp critical
            {
                odd_prime_number_pairs.insert(i);
                odd_prime_number_pairs.insert(i + 2);
                // std::cout << "[" << std::setw(2) << omp_get_thread_num() << "] " << i << " " << i + 2 << "\n";
            }
            #pragma omp atomic
            no_of_pairs++;
        }
    }
    std::cout << "count: "<< no_of_pairs << "\nsum : ";
    std::cout << std::accumulate(odd_prime_number_pairs.begin(), odd_prime_number_pairs.end(), 0);
    std::cout << "\n";
}

int main() {
    //omp_set_num_threads(4);
    const int count = 2'00'000;
    auto s = TIME_POINT;
    run(count);
    auto m = TIME_POINT;
    parallel_run(count);
    auto e = TIME_POINT;
    std::cout << "total run time for serial   : " << RUN_TIME(s, m) << std::endl;
    std::cout << "total run time for parallel : " << RUN_TIME(m, e) << std::endl;
    
    return 0;
}