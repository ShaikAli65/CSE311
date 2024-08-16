/*
5. Write an openMP program (C++) for the following sorting algorithms with serial and parallel
computation and note the execution time of the same. Analyse the output with different high
N values, where N is the number of elements to be sorted. (Like above table)
    a) Merge sort
    b) Quick sort
*/


#include <omp.h>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <thread>
#include <cassert>

#define TIME_POINT(id) const auto id =  std::chrono::high_resolution_clock::now()
#define RUN_TIME(prefix, start_id, end_id) std::cout << "\n" << prefix <<\
 (static_cast<std::chrono::duration<double>>(end_id - start_id)).count() << std::endl

#define THRESHOLD 400

template<typename T>
void print(const std::vector<T> &in) {
    
    std::cout << "[";
    for(const auto&i : in) {
        std::cout << i << ", ";
    }
    std::cout << "]";

    std::cout << std::endl;
}


namespace quick
{
    int _partition(const int &low, const int &high, std::vector<int> &in) {
        int pivot = in[high];
        int i = low - 1, j = low;
        while (j < high)
        {
            if(in[j] < pivot) {
                i++;
                std::swap(in[i], in[j]);
            }
            j++;
        }
        std::swap(in[i + 1], in[high]);
        return i + 1;
    }

    void qsort(std::vector<int> &in, const int &low, const int &high) {
        if (low >= high) {
            return;
        }
        const int &pivot = _partition(low, high, in);
        qsort(in, low, pivot - 1);
        qsort(in, pivot + 1, high);
    }

    void pqsort(std::vector<int> &in, const int &low, const int &high) {
        if (low >= high) {
            return;
        }
        const int &pivot = _partition(low, high, in);
        if (high - low < THRESHOLD) {
            // for small partitions, 
            // to incur less threading overhead
            qsort(in, low, high);
            return;
        }
        #pragma omp taskgroup
        {
            #pragma omp task shared(in) firstprivate(low, pivot)
            pqsort(in, low, pivot - 1);

            #pragma omp task shared(in) firstprivate(high, pivot)
            pqsort(in, pivot + 1, high);
        }
    }
    void sort(std::vector<int> &in) {
        qsort(in, 0, in.size() - 1);
    }
    void psort(std::vector<int> &in) {
        #pragma omp parallel
        #pragma omp single
        pqsort(in, 0, in.size() - 1);
    }
} // namespace quick


namespace merge
{
    void _merge_inplace(std::vector<int> &in, const int &left, const int &mid, const int &right) {
        std::vector<int> merged(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        while (i <= mid && j <= right)
        {
            if(in[i] < in[j]) {
                merged[k++] = in[i++]; 
            }
            else {
                merged[k++] = in[j++];
            }
        }
        while (i <= mid)
        {
            merged[k++] = in[i++];
        }
        while (j <= right)
        {
            merged[k++] = in[j++];
        }
        std::copy(merged.begin(), merged.end(), in.begin() + left);
    }

    void merge(std::vector<int> &in, const int left, const int right) {
        if (left >= right) {
            return;
        }

        const int mid = left + (right - left) / 2;

        merge(in, left, mid);
        merge(in, mid + 1, right);
        _merge_inplace(in, left, mid, right);
    }

    void _pmerge(std::vector<int> &in, const int &left, const int &right) {
        if (left >= right) 
            return;
        
        const int &mid = left + (right - left) / 2;

        if (right - left > THRESHOLD) {
            #pragma omp taskgroup
            {
                #pragma omp task shared(in) firstprivate(left, mid)
                _pmerge(in, left, mid);

                #pragma omp task shared(in) firstprivate(mid, right)
                _pmerge(in, mid + 1, right);
                // #pragma omp taskyield --> this thread can be suspended 
                // but we are using taskwait down
            }
        } else {
            // For small arrays, using sequential processing
            merge(in, left, mid);
            merge(in, mid + 1, right);
        }

        #pragma omp taskwait 
        _merge_inplace(in, left, mid, right);
    }

    void psort(std::vector<int> &in) {
        #pragma omp parallel
        #pragma omp single
        _pmerge(in, 0, in.size() - 1);
    }

    void sort(std::vector<int> &in) {
        merge(in, 0, in.size() - 1);
    }    

} // namespace merge


void fill_vector(std::vector<int> &in){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dists;
    for(auto &a: in) {
        a = dists(gen);
    }
}

bool check_sorted(const std::vector<int> &v) {
    for(int i = 0, j = 1; j < v.size();i++, j++) {
        if (v[i] > v[j])
        {
            return false;
        }
    }
    return true;
}

void run() {
    std::vector<int> v;//= {2, 1, 27, 13, 11, 4, 54, 100, 7};
    // print(v);
    v.resize(10);
    // v.resize(100);
    // v.resize(1000);
    // assert(check_sorted(v));
    fill_vector(v);
    TIME_POINT(s1);
    merge::psort(v);
    TIME_POINT(s2);
    RUN_TIME("MPsort time: ", s1, s2);
    fill_vector(v);
    merge::sort(v);
    TIME_POINT(s3);
    RUN_TIME("Msort time: ", s2, s3);
    fill_vector(v);
    quick::sort(v);
    TIME_POINT(s4);
    RUN_TIME("Qsort time: ", s3, s4);
    fill_vector(v);
    quick::psort(v);
    TIME_POINT(s5);
    RUN_TIME("QPsort time: ", s4, s5);
    
    // print(v);
}

int main() {
    //omp_set_num_threads(4);
    std::cin.tie(0);
    std::ios::sync_with_stdio(false);
    TIME_POINT(s);
    run();
    TIME_POINT(e);
    RUN_TIME("total run time:", s, e);
    return 0;
}

/*
Execution times (sec):
    ╭────────────────────────────────────────────────────────────────────╮
    │ Len  │   Merge   │   Parallel Merge  │   Quick   │  Parallel Quick │
    │ ──── + ───────── + ───────────────── + ───────── + ─────────────── │
    │ 10   │ 0.0001155 │     0.0013929     │  3.6e-05  │   0.000275      │
    │ ──── + ───────── + ───────────────── + ───────── + ─────────────── │
    │ 100  │ 0.0002286 │     0.0027847     │  0.000115 │   0.0006319     │
    │ ──── + ───────── + ───────────────── + ───────── + ─────────────── │
    │ 1000 │ 0.0004848 │     0.0020288     │ 0.0003093 │   0.0006246     │
    ╰────────────────────────────────────────────────────────────────────╯
*/
