/*
a) Write an OpenMP serial and parallel program with C/C++ to compute the
occurrence of words in a file.
b) Store the counts of each words from (a) in an array. Using OpenMP, do the parallel
version of Quicksort to arrange the counts into descending order and then display top
10 words based on the highest count. 
*/
#include <iostream>
#include <fstream>
#include <ios>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cstdint>

uint64_t get_no_of_lines(std::ifstream &f) {
    uint64_t s = f.tellg();
    f.seekg(0);
    f.unsetf(std::ios_base::skipws);
    unsigned line_count = std::count(
        std::istream_iterator<char>(f),
        std::istream_iterator<char> (),
        '\n'
    );
    f.setf(std::ios_base::skipws);
    f.clear();
    f.seekg(s);
    return line_count;
}

int main() {
    std::ifstream f("chandrayaan.txt"); 
    if (!f.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return -1;
    }
    uint64_t line_count = get_no_of_lines(f);
    std::unordered_map<std::string, uint64_t> m;

    #pragma omp parallel
    {
        std::unordered_map<std::string, uint32_t> local_count;
        
        #pragma omp for
        for (size_t i = 0; i < line_count; i++)
        {
            std::string s;
            #pragma omp critical
            getline(f,s);
            std::istringstream iss(s);
            std::string word;
            while (iss >> word) {
                #pragma omp atomic
                local_count[word]++;
            }
        }

        for (auto &&i : local_count)
        {
            m[i.first] += i.second;
        }
    }
    for (auto &&i : m)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }
    
}