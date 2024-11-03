// lalith dutt
#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>
using namespace std;
bool isPrime(long long int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (long long int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}
void isPerfect(int N)
{
    vector<long long int> results(N);
#pragma omp parallel
    {
#pragma omp for ordered
        for (int i = 1; i <= N; i++)
        {
            long long int x = (1LL << i) - 1;
            if (isPrime(x))
            {
                long long int y = (1LL << (i - 1));
                long long int perfectNum = x * y;
#pragma omp ordered
                results[i - 1] = perfectNum;
            }
        }
    }
    cout << "Parallel Version :" << endl;
    for (const auto &res : results)
    {
        if (res != 0)
            cout << res << endl;
    }
}
int main()
{
    int N = 100;
    isPerfect(N);
    return 0;
}