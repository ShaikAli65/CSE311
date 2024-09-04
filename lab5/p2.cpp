/*
2.
a) Write a C++ program for serial bubble sort. What is the asymptotic time complexity
for this algorithm if N is the size of the array?
Test Case: N=10, Unsorted Array: 19 2 72 3 18 57 603 490 45 101
 Expected Output: 2 3 18 19 45 57 72 101 490 603
b) Write C++ program for odd-even transposition sort, which is a variant of bubble
sort. Verify the algorithm for the above test case. What is the asymptotic time
complexity for this algorithm if N is the size of the array?
c) Write an OpenMP program with C++ for Parallel odd-even transposition sort, which
is a variant of bubble sort. Verify the algorithm for the above test case. What is the
asymptotic time complexity for this algorithm if N is the size of the array, and P is the
total number of threads used?
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iomanip>
#define TIME_POINT omp_get_wtime()
#define RUN_TIME(start_id, end_id) end_id - start_id
// #define TIME_POINT std::chrono::high_resolution_clock::now()
// #define RUN_TIME(start_id, end_id) (static_cast<std::chrono::duration<double>>(end_id - start_id)).count()

std::vector<double> testCase(const int &n)
{
    std::vector<double> v(n);
    std::srand(time(0));
    std::generate(v.begin(), v.end(), std::rand);
    return v;
}
std::string line = "\n" + std::string(31,'=') + "\n";

std::vector<double> serialBubbleSort(std::vector<double> &in) {
    auto s = TIME_POINT;
    double swaps = 0;
    for (size_t i = 0; i < in.size(); i++)
    {
        for (size_t j = i; j < in.size(); j++)
        {
            if (in[i] > in[j])
            {
                std::swap(in[i], in[j]);
                swaps++;
            }
        }
    }
    auto e = TIME_POINT;
    std::cout << line;
    std::cout << "::bubble sort"; 
    std::cout << line << "\n";
    std::cout << "number of elements :" << in.size() << "\n"; 
    std::cout << "no of swaps        :" << swaps<< "\n"; 
    std::cout << "run time           :" << RUN_TIME(s, e) << "\n";
    std::cout << line << "\n";
    return {swaps, RUN_TIME(s, e)};
}

std::vector<double> oddEvenTranspositionSort(std::vector<double> &in) {
    bool isSorted = false;
    int n = in.size();
    double no_of_odd_swaps = 0, no_of_even_swaps = 0;
    auto s = TIME_POINT;
    while (!isSorted)
    {
        isSorted = true;
        for (size_t i = 1; i <= n - 2; i += 2)
        {
            if (in[i] > in[i+1]) {
                std::swap(in[i], in[i + 1]);
                no_of_odd_swaps += 1;
                isSorted = false;
            }
        }
        for (size_t i = 0; i <= n - 2; i += 2)
        {
            if (in[i] > in[i+1]) {
                std::swap(in[i], in[i + 1]);
                no_of_even_swaps += 1;
                isSorted = false;
            }            
        }
    }
    auto e = TIME_POINT;
    std::cout << line;
    std::cout << "::odd-even sort"; 
    std::cout << line << "\n";
    std::cout << "number of elements :" << in.size() << "\n"; 
    std::cout << "no of even swaps   :" << no_of_even_swaps << "\n"; 
    std::cout << "no of odd swaps    :" << no_of_odd_swaps << "\n"; 
    std::cout << "run time           :" << RUN_TIME(s, e) << "\n";
    std::cout << line << "\n";
    return {no_of_even_swaps, no_of_odd_swaps, RUN_TIME(s, e)};
}

std::vector<double> oddEvenTranspositionSortParallel(std::vector<double> &in) {
    bool isSorted = false;
    int n = in.size();
    double no_of_odd_swaps = 0, no_of_even_swaps = 0;
    auto s = TIME_POINT;
    while (!isSorted)
    {
        isSorted = true;
        #pragma omp parallel for shared(isSorted) reduction(+:no_of_even_swaps)
        for (size_t i = 1; i <= n - 2; i += 2)
        {
            if (in[i] > in[i+1]) {
                std::swap(in[i], in[i + 1]);
                no_of_odd_swaps++;
                isSorted = false;
            }
        }

        #pragma omp parallel for shared(isSorted) reduction(+:no_of_even_swaps)
        for (size_t i = 0; i <= n - 2; i += 2)
        {
            if (in[i] > in[i+1]) {
                std::swap(in[i], in[i + 1]);
                no_of_even_swaps++;
                isSorted = false;
            }            
        }
    }
    auto e = TIME_POINT;
    std::cout << line;
    std::cout << "::Parallel odd-even sort";
    std::cout << line << "\n";
    std::cout << "number of elements :" << in.size() << "\n"; 
    std::cout << "no of even swaps   :" << no_of_even_swaps << "\n"; 
    std::cout << "no of odd swaps    :" << no_of_odd_swaps << "\n"; 
    std::cout << "run time           :" << RUN_TIME(s, e) << "\n";
    std::cout << line << "\n";
    return {no_of_even_swaps, no_of_odd_swaps, RUN_TIME(s, e)};
}

void run() {
    std::vector<int> n_values = {10, 50, 100, 500};//, 1000, 5000};
    std::vector<double> t;
    for (auto &&i : n_values)
    {
        t = testCase(i);
        auto s = serialBubbleSort(t);
        t = testCase(i);
        s = oddEvenTranspositionSort(t);
        t = testCase(i);
        s = oddEvenTranspositionSortParallel(t);
    }
}

int main() {
    run();
    return 0;
}