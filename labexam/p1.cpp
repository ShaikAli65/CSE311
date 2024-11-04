#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iomanip>



double monteCarloParallel(int points)
{
    int circlePoints;
    srand(time(0));
    #pragma omp parallel for //reduction(+:circlePoints)
    for (int i = 0; i < points; i++)
    {
        double randX = static_cast<double>(rand()) / RAND_MAX;
        double randY = static_cast<double>(rand()) / RAND_MAX;
        if ((randX * randX) + (randY * randY) <= 1.0)
        {
#pragma omp critical
            circlePoints++;
        }
    }
    return 4.0 * circlePoints / points;
}

int main() {
    auto s = omp_get_wtime();
    int n = 10000;
    auto pie = monteCarloParallel(n);
    std::cout << "PIe value "<< pie << "\n";
    auto e = omp_get_wtime();
    std::cout << "Run time " << e -s << "\n";
}