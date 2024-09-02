/*
Write an OpenMP program with C++ that estimates the value of pi (𝜋) using a
following function and apply rectangle rule.
𝑨𝒓𝒆𝒂 = ∫ 𝒇(𝒙)
𝒃
𝒂
𝒅𝒙 , 𝒘𝒉𝒆𝒓𝒆 𝒇(𝒙) =
𝟒
𝟏 + 𝒙
𝟐
𝒂 = 𝟎, 𝒃 = 𝟏𝟎,𝒏 = 𝟏𝟎, 𝟓𝟎, 𝟏𝟎𝟎, 𝟓𝟎𝟎, 𝟏𝟎𝟎𝟎.

*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#define REAL_PI 
#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl
#define PARALLEL

const double pi = 3.14159265358979323846;

double f(double x) {
    return 4.0 / (1.0 + (x * x));
}

double a = 0, b = 1; 
double n = 1'000;

double calculate_pi(const double &n, const double &alignment, const std::string &detail="")
{   
    double width = (b - a) / n;
    double area = 0.0;

    double start = omp_get_wtime();

#ifdef PARALLEL
    #pragma omp parallel for shared(n) reduction(+: area)
#endif
    for(int i = 0; i < int(n); i++) {
        double x = a + width * (i + alignment);
        area += f(x) * width;
    }

    double end = omp_get_wtime();
    
//     std::string serial_or_parallel = "serial";
// #ifdef PARALLEL
//     serial_or_parallel = "parallel";
// #endif
//     std::cout << "Value of pi(" << serial_or_parallel <<"):" << area << std::endl;
//     double error = pi - area;
//     std::cout << "Error value(" << serial_or_parallel <<"):" << error << std::endl;
//     std::cout << "Execution time(" << detail <<"):" << (end - start) << std::endl;
    return area;
}

void print_stats(const std::string &prefix,const std::initializer_list<double> &n_values, const std::vector<std::vector<double>> &stats) {
    std::cout << "Actual PI " << std::setprecision (15) << pi << "\n";
    std::string line(20 * n_values.size() + 7,'-');
    line = "\n" + line + "\n";
    
    std::cout << line;
    std::cout << '|';
    std::cout  <<  std::setw(10) << prefix << "|";
    for (auto &&i : n_values)   
    {
        std::cout << std::setw(10) << i << std::setw(9) <<  '|';
    }
    std::cout << line;
    std::string prefixes[] = {"start", "middle", "end"};
    for (size_t i = 0; i < stats.size(); i++)
    {
        std::cout << "| " << std::setw(9) << std::left << prefixes[i] <<"| ";
        for (size_t j = 0; j < stats[0].size(); j++)
        {
            std::cout << std::setw(16) <<stats[i][j] << " | ";
        }
        std::cout << line;
    }
}

void serial_pi()
{    
    std::initializer_list<double> alignments = {0.0, 0.5, 1};
    std::initializer_list<double> n_values = {10, 50, 100, 500, 1000};
    std::vector<std::vector<double>> stats(3, std::vector<double>(n_values.size()));

    int i = 0, j = 0;

    for (auto &&alignment : alignments)
    {
        for (auto &&n : n_values)
        {
            stats[i][j++] = calculate_pi(n, alignment);
        }
        i++;
        j = 0;
    }

    print_stats(" serial",n_values, stats);
}

void parallel_pi()
{
    serial_pi();
}

void run()
{
    serial_pi();
    // parallel_pi();
}

int main() {
    run();
    return 0;
}