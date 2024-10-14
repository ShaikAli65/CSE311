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
#include <ios>

// #define TIME_POINT std::chrono::high_resolution_clock::now()
// #define RUN_TIME(start_id, end_id) static_cast<std::chrono::duration<double>>(end_id - start_id).count()
#define TIME_POINT omp_get_wtime()
#define RUN_TIME(start_id, end_id) (end_id - start_id)

const double pi = 3.14159265358979323846;

double f(double x) {
    return 4.0 / (1.0 + (x * x));
}

double a = 0, b = 1; 
std::initializer_list<double> alignments = {0.0, 0.5, 1};
std::initializer_list<double> n_values = {10, 50, 100, 500, 1000};
std::vector<std::vector<double>> stats_pi(3, std::vector<double>(n_values.size()));
std::vector<std::vector<double>> stats_durations(3, std::vector<double>(n_values.size()));
std::vector<std::vector<double>> stats_errors(3, std::vector<double>(n_values.size()));

void printStats(
    const std::string &prefix,
    const std::initializer_list<double> &n_values,
    const std::vector<std::vector<double>> &stats
) {
    std::string line(20 * n_values.size() + 7,'-');
    line = "\n" + line + "\n";
    std::ios_base::fmtflags f(std::cout.flags());
    std::cout << line;
    std::cout << '|' << std::left <<  std::setw(10)  << prefix << "|";
    std::cout.flags(f);
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
            // std::cout << std::setw(16) << std::fixed << stats[i][j] << " | ";
            std::cout << std::setw(16) << stats[i][j] << " | ";
        }
        std::cout << line;
    }
    std::cout.flags(f);
}

auto calculatePi(const double &n, const double &alignment)
{   
    double width = (b - a) / n;
    double area = 0.0;

    double start = TIME_POINT;

    for(int i = 0; i < int(n); i++) {
        double x = a + width * (i + alignment);
        area += f(x) * width;
    }

    double end = TIME_POINT;
    return std::pair<double, double >{area, RUN_TIME(start, end)};
}

auto calculatePiParallel(const double &n, const double &alignment) 
{   
    double width = (b - a) / n;
    double area = 0.0;

    double start = TIME_POINT;
    #pragma omp parallel for shared(n) reduction(+: area)
    for(int i = 0; i < int(n); i++) {
        double x = a + width * (i + alignment);
        area += f(x) * width;
    }

    double end = TIME_POINT;
    return std::pair<double, double> {area, RUN_TIME(start, end)};
}

auto calculatePiParallelWithRace(const double &n, const double &alignment) 
{   
    double width = (b - a) / n;
    double area = 0.0;

    double start = TIME_POINT;
    #pragma omp parallel for shared(n)
    for(int i = 0; i < int(n); i++) {
        double x = a + width * (i + alignment);
        area += f(x) * width;
    }
    double end = TIME_POINT;
    return std::pair<double, double> {area, RUN_TIME(start, end)};
}


void serialPi()
{    
    int i = 0, j = 0;
    for (auto &&alignment : alignments)
    {
        for (auto &&n : n_values)
        {
            auto [area, duration] = calculatePi(n, alignment);
            stats_pi[i][j] = area; 
            stats_durations[i][j] = duration;
            stats_errors[i][j++] = pi - area;
        }
        i++;
        j = 0;
    }
    std::cout << "Actual PI " << std::setprecision (10) << pi << "\n";
    std::cout << "\nserial\n";
    printStats(" values", n_values, stats_pi);

    std::cout << "\nserial\n";
    printStats(" durations", n_values, stats_durations);
    std::cout << "\nserial\n";
    printStats(" errors", n_values, stats_errors);
}

void parallelPi()
{
    int i = 0, j = 0;

    for (auto &&alignment : alignments)
    {
        for (auto &&n : n_values)
        {
            auto [area, duration] = calculatePiParallel(n, alignment);
            stats_pi[i][j] = area; 
            stats_durations[i][j] = duration; 
            stats_errors[i][j++] = pi - area; 
        }
        i++;
        j = 0;
    }
    
    std::cout << "Actual PI " << std::setprecision (10) << pi << "\n";
    std::cout << "\nparallel\n";
    printStats(" values", n_values, stats_pi);

    std::cout << "\nparallel\n";
    printStats(" durations", n_values, stats_durations);
    std::cout << "\nparallel\n";
    printStats(" errors", n_values, stats_errors);
}

void parallelPiWithRace() 
{
    int i = 0, j = 0;

    for (auto &&alignment : alignments)
    {
        for (auto &&n : n_values)
        {
            auto [area, duration] = calculatePiParallelWithRace(n, alignment);
            stats_pi[i][j] = area; 
            stats_durations[i][j] = duration; 
            stats_errors[i][j++] = pi - area; 
        }
        i++;
        j = 0;
    }

    std::cout << "\n\n\nActual PI " << std::setprecision (10) << pi << "\n";
    std::cout << "PARALLEL PI WITH RACE\n";
    printStats(" values", n_values, stats_pi);
    printStats(" durations", n_values, stats_durations);
    printStats(" errors", n_values, stats_errors);
}
int main() {
    serialPi();
    parallelPi();
    parallelPiWithRace();
    return 0;
}