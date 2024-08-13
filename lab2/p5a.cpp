/*
5. Write an openMP program (C++) for the following sorting algorithms with serial and parallel
computation and note the execution time of the same. Analyse the output with different high
N values, where N is the number of elements to be sorted. (Like above table)
a) Merge sort
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl

template<typename T>
void sort(std::vector<T> &in) {

}


template<typename T>
void fill_vector(std::vector<T> &in){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dists;
    for(auto &a: in) {
        a = dists(gen);
    }
}

void run() {
    std::vector<int> v = {1,2,3,2,31,45,4,2,87,6};
    
}

int main() {
    //omp_set_num_threads(4);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}