#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
void run() {

}
int main() {
    omp_set_num_threads(4);
    auto s = std::chrono::high_resolution_clock::now();
    run();
    auto e = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> d = e - s;
    std::cout << "\nrun time :" << d.count() << "s\n";
    return 0;
}

