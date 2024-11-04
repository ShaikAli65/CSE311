#include <iostream>
#include <vector>
#include <cmath>

// Function to check if a number is prime
bool isPrime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

// Function to find the first n perfect numbers using Mersenne primes
std::vector<long long> findPerfectNumbers(int n) {
    std::vector<long long> perfectNumbers;
    int count = 0;
    int p = 2;

    while (count < n) {
        // Check if 2^p - 1 is prime
        long long mersennePrime = pow(2, p - 1) - 1; // 2^p - 1
        if (isPrime(mersennePrime)) {
            // If it's prime, calculate the perfect number
            long long perfectNumber = pow(2, p - 1) * mersennePrime; // 2^(p-1) * (2^p - 1)
            perfectNumbers.push_back(perfectNumber);
            count++;
        }
        p++;
    }

    return perfectNumbers;
}

int main() {
    int n;
    std::cout << "Enter the number of perfect numbers to find: ";
    std::cin >> n;

    std::vector<long long> perfectNumbers = findPerfectNumbers(n);

    std::cout << "First " << n << " perfect numbers: ";
    for (long long num : perfectNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
