#include <mpi.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include "transpose"
using namespace std;


double localResult(vector<int> &a, vector<int> &b)
{
    int sum = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        sum += a[i] * b[i];
    }
    return sum;
}

void print_arr(vector<int> &a) {
    for (auto &&i : a)
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    
    int rows1 = 3;
    int cols1 = 2;
    vector<int> a(rows1 * cols1);
    
    int rows2 = 2;
    int cols2 = 3;
    vector<int> b(rows2 * cols2);

    vector<int> locala(cols1);
    vector<int> localb(rows2);
    
    if (world_rank == 0) {
        vector<int> input1 =  {
            2, -1,
            0, 3,
            4, 5,
            }; 
        for (size_t i = 0; i < input1.size(); i++)
        {
            a[i] = input1[i];
        }
        
        vector<int> input2 = {
            1,-1,
            0,3,
            2,1
        };       

        for (size_t i = 0; i < input2.size(); i++)
        {
            b[i] = input2[i];
        }
    }

    MPI_Bcast(a.data(), a.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b.data(), cols1, MPI_INT, localb.data(), rows2, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> local_result;
    for (size_t i = 0; i < a.size(); i+= localb.size())
    {
        vector<int> localv;
        for (size_t j = 0; j < localb.size(); j++)
        {
            localv.emplace_back(a[i + j]);
        }
        auto result = localResult(localb, localv);
        local_result.push_back(result);
    }
    std::cout << "process[" << world_rank << "] calculating result : "; 
    print_arr(local_result);

    if (world_rank != 0) {
        MPI_Send(local_result.data(),local_result.size(),MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (world_rank == 0) {
        vector<vector<int>> matrix;
        matrix.emplace_back(local_result);
        for (size_t i = 1; i < rows1; i++)
        {
            vector<int> partial(rows1);
            MPI_Recv(partial.data(), rows1, MPI_INT,i,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            matrix.emplace_back(partial);
        }
        for (size_t i = 0; i < matrix[0].size(); i++)
        {
            for (size_t j = 0; j < matrix.size(); j++)
            {
                std::cout << matrix[j][i] << " ";
            }
            std::cout << "\n";
        }
    }

    MPI_Finalize();
}