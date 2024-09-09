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
        // std::cout << "a" << in;
        if (in % i == 0)
        {
            return false;
        }
    }
    return true;
}


void run() {
    const int count = 200;
    std::vector<int> prime_numbers;
    std::vector<std::vector<int>> consecutive_odd_primes(1, std::vector<int> {});
    for (int i = 3; i < count; i += 2)
    {
        if(isprime(i))
        {
            prime_numbers.push_back(i);
        }
    }
    int j = 0;
    int c = 0;
    int prev_prime = 3;
    std::unordered_set<int> elements;
    for (size_t i = 0; i < prime_numbers.size(); i++)
    {
        if (prime_numbers[i] == prev_prime + 2){
            c++;
            elements.emplace_hint(std::begin(elements), prev_prime);
            elements.emplace_hint(std::begin(elements), prev_prime + 2);
            std::cout << prev_prime << " " << prev_prime + 2 << "\n"; 
        }
        prev_prime = prime_numbers[i];
    }
    std::cout << "count: "<< c << "\nsum : ";
    std::cout << std::accumulate(elements.begin(), elements.end(), 0);
    std::cout << "\n";
}
void parallel_run() {
    const int count = 200;
    std::vector<int> prime_numbers;
    std::vector<std::vector<int>> consecutive_odd_primes(1, std::vector<int> {});
    // #pragma omp parallel for 
    for (int i = 3; i < count; i += 2)
    {
        if(isprime(i))
        {
            prime_numbers.push_back(i);
        }
    }
    int j = 0;
    int c = 0;
    int prev_prime = 3;
    std::unordered_set<int> elements;
    #pragma omp parallel for shared(elements)
    for (size_t i = 0; i < prime_numbers.size(); i++)
    {
        #pragma omp critical
        {
            if (prime_numbers[i] == prev_prime + 2){
                    c++;
                    elements.emplace_hint(std::begin(elements), prev_prime);
                    elements.emplace_hint(std::begin(elements),  prev_prime + 2);
                    std::cout << prev_prime << " " << prev_prime + 2 << "\n"; 
            }
            prev_prime = prime_numbers[i];
        }
    }
    std::cout << "count: "<< c << "\nsum : ";
    std::cout << std::accumulate(elements.begin(), elements.end(), 0);
    std::cout << "\n";
}

int main() {
    //omp_set_num_threads(4);
    auto s = TIME_POINT;
    run();
    parallel_run();
    auto e = TIME_POINT;
    std::cout << "total run time: " << RUN_TIME(s, e) << std::endl;
    return 0;
}