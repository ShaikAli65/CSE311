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

void gatherAndPrint(std::vector<int> local_arr, int local_n, int finalArrSize, int phase, int rank) {
    std::vector<int> finalArray;

    if (rank == 0) {
        finalArray.resize(finalArrSize);
    }
    
    MPI_Gather(local_arr.data(), local_n, MPI_INT, finalArray.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << ((phase % 2 == 0) ? "even" : "odd") 
                  << " phase\n";
                  
        printArray(finalArray);
    }
}

void oddEvenSortParallel(std::vector<int> &local_arr, int local_n, int rank, int size, int n) {
    int sorted;

    for (size_t phase = 0; phase < n; phase++) {
        sorted = 1;
        int neighbor_val;

        // Even phase
        if (phase % 2 == 0) {
            if (rank % 2 == 0 && rank < size - 1) {
                MPI_Sendrecv(
                    &local_arr[local_n - 1], 1, MPI_INT, rank + 1, 0, // send
                    &neighbor_val, 1, MPI_INT, rank + 1, 0,  // recv
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE
                );

                if (local_arr[local_n - 1] > neighbor_val) {
                    local_arr[local_n - 1] = neighbor_val;
                    sorted = 0;
                }

            } else if (rank % 2 != 0 && rank > 0) {

                MPI_Sendrecv(
                    &local_arr[0], 1, MPI_INT, rank - 1, 0,  // send
                    &neighbor_val, 1, MPI_INT, rank - 1, 0,  // recv
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE
                );
                
                if (local_arr[0] < neighbor_val) {
                    local_arr[0] = neighbor_val;
                    sorted = 0;
                }
            }
        }

        // Odd phase
        else {
            if (rank % 2 != 0 && rank < size - 1) {
                MPI_Sendrecv(
                    &local_arr[local_n - 1], 1, MPI_INT, rank + 1, 0,
                    &neighbor_val, 1, MPI_INT, rank + 1, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE
                );
                
                if (local_arr[local_n - 1] > neighbor_val) {
                    local_arr[local_n - 1] = neighbor_val;
                    sorted = 0;
                }
            } 
            else if (rank % 2 == 0 && rank > 0) {
                MPI_Sendrecv(
                    &local_arr[0], 1, MPI_INT, rank - 1, 0,
                    &neighbor_val, 1, MPI_INT, rank - 1, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE
                );
                if (local_arr[0] < neighbor_val) {
                    local_arr[0] = neighbor_val;
                    sorted = 0;
                }
            }
        }

        // Local sort within each process
        for (size_t i = 0; i < local_n - 1; i++) {
            if (local_arr[i] > local_arr[i + 1]) {
                int temp = local_arr[i];
                local_arr[i] = local_arr[i + 1];
                local_arr[i + 1] = temp;
                sorted = 0;
            }
        }

        int global_sorted;
        
        MPI_Allreduce(&sorted, &global_sorted, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

        gatherAndPrint(local_arr, local_n, n, phase, rank);

        if (global_sorted) break;
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int processId, noOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &noOfProcesses);

    int n;
    std::vector<int> arr;

    int local_n;

    if (processId == 0) {
        // using test case given in question
        n = 16;
        arr.assign({15, 29, 100, 23, -14, 45, 178, 192, 246, -118, 0, 7, 1, 10, 25, 34});
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_n = n / noOfProcesses;

    std::vector<int> local_arr(local_n);
    MPI_Scatter(
        arr.data(), local_n, MPI_INT,
        local_arr.data(), local_n, MPI_INT,
        0, MPI_COMM_WORLD
    );

    oddEvenSortParallel(local_arr, local_n, processId, noOfProcesses, n);

    MPI_Gather(local_arr.data(), local_n, MPI_INT, arr.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);


    if (processId == 0) {
        std::cout << "Final sorted array:\n";
        printArray(arr);
    }

    MPI_Finalize();
    return 0;
}