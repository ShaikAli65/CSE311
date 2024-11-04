// #include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    std::vector<std::vector<int>> m = {
        {1,2,3},
        {4,5,6},
        {7,8,9},
    };
    std::cout << m[2][1]; 
}
