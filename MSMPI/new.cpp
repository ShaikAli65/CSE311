#include <vector>
#include <iostream>
#include <mpi.h>

void printArray(std::vector<int> &arr) {
    for (auto &&i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int processId, noOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &noOfProcesses);
    
    std::vector<int> v = {1,2,3};
    
    MPI_Bcast(v.data(),2,MPI_INT,0, MPI_COMM_WORLD);
    printArray(v);
    MPI_Finalize();
}