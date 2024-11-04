/*
1. Write a MPI program that estimates the value of the integral using trapezoidal rule
for numerical integration.
𝑨𝒓𝒆𝒂 = ∫ 𝒇(𝒙)
𝒃
𝒂
𝒅𝒙 , 𝒘𝒉𝒆𝒓𝒆 𝒇(𝒙) = 𝟐𝒙 + 𝟑 𝒂 = 𝟎, 𝒃 = 𝟐, 𝒏 = 𝟏𝟔, 𝟏𝟐𝟖, 𝟓𝟏𝟐, 𝟏𝟎𝟐𝟒.
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>


#define TIME_POINT std::chrono::high_resolution_clock::now(),
#define RUN_TIME(start_id, end_id) (static_cast<std::chrono::duration<double>>(end_id - start_id)).count(),


double f(double x) {
    return 2 * x + 3; 
}

double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.5 * (f(a) + f(b)); 

    for (int i = 1; i < n; i++) {
        auto x = a + i * h;
        integral += f(x); 
    }
    
    return integral * h;
}


int main() {
    int a = 0;
    int b = 2;
    std::vector<uint16_t> n = {16, 128, 512, 1024};
    for (auto &&i : n)
    {
        auto v = trapezoidal_rule(a, b, i);
        std::cout << v << "\n"; 
    }
    
}