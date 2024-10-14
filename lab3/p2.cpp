/*
Write an openMP program with C++ that estimates the value of pi (𝜋) using MonteCarlo simulation.
Case 1: Consider the circle of radius r >=1 say r=100 which drawn inside the Square
Case 2: Consider the quarter circle with radius r=1 which drawn inside the Unit square.
Perform the following for the above cases:
*/

#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iomanip>
#define TIME_POINT omp_get_wtime()
#define RUN_TIME(start_id, end_id) end_id - start_id
// #define TIME_POINT std::chrono::high_resolution_clock::now()
// #define RUN_TIME(start_id, end_id) (static_cast<std::chrono::duration<double>>(end_id - start_id)).count()

double monteCarloSerial(int points)
{
    int circlePoints = 0;
    srand(time(0));
    for (int i = 0; i < points; i++)
    {
        double randX = static_cast<double>(rand()) / RAND_MAX;
        double randY = static_cast<double>(rand()) / RAND_MAX;
        if ((randX * randX) + (randY * randY) <= 1.0)
            circlePoints++;
    }
    return 4.0 * circlePoints / points;
}

double monteCarloParallel(int points)
{
    int circlePoints = 0;
    srand(time(0));
    #pragma omp parallel for
    for (int i = 0; i < points; i++)
    {
        double randX = static_cast<double>(rand()) / RAND_MAX;
        double randY = static_cast<double>(rand()) / RAND_MAX;
        if ((randX * randX) + (randY * randY) <= 1.0)
            circlePoints++;
    }
    return 4.0 * circlePoints / points;
}

double monteCarloParallelWithoutRace(int points)
{
    int circlePoints = 0;
    srand(time(0));
    #pragma omp parallel for reduction(+: circlePoints)
    for (int i = 0; i < (points); i++)
    {
        double randX = static_cast<double>(rand()) / RAND_MAX;
        double randY = static_cast<double>(rand()) / RAND_MAX;
        if ((randX * randX) + (randY * randY) <= 1.0)
            #pragma omp critical
            circlePoints++;
    }
    return 4.0 * circlePoints / points;
}
void run(int n)
{
    std::string line(40, '-');
    line = "\n" + line + "\n";
    if (n >= 1000)
        std::cout << "<=for n :" << n << std::string(55,'=') << "\n\n";
    else
        std::cout << "<=for n :" << n << std::string(57,'=') << "\n\n";
    auto s = TIME_POINT;
    double result = monteCarloSerial(n);
    auto e = TIME_POINT;
    std::cout << "pi value for serial: " << result << std::endl;
    std::cout << "execution time(serial) : " << RUN_TIME(s, e) << "s\n";
    std::cout << line;
    s = TIME_POINT;
    result = monteCarloParallel(n);
    e = TIME_POINT;
    std::cout << "\npi value for parallel:" << result << 
    "\nExecution time : " << RUN_TIME(s, e) << "s\n";
    std::cout << line;
    s = TIME_POINT;
    result = monteCarloParallelWithoutRace(n);
    e = TIME_POINT;
    std::cout << "\npi value for parallel(reduction):" << result << 
    "\nExecution time(reduction) : " << RUN_TIME(s, e) << "s\n";

    std::cout << "\n" <<std::string (69, '=') <<"\n\n\n";
}

int main()
{
    omp_set_num_threads(5);
    std::vector<int> n = {100, 1000, 10000};
    for (int i : n)
    {
        run(i);
    }
    return 0;
}