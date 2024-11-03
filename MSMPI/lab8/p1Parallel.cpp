        gather_and_print(local_arr, local_n, n, phase, rank, size); 
         if (global_sorted) break; 
    } 
}  int main(int argc, char** argv) {     int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
     int n;     int *arr = NULL;     int local_n; 
     if (rank == 0) {         printf("Enter the number of elements: ");         scanf("%d", &n); 
         if (n % size != 0) {             printf("Number of elements must be divisible by the number of processes.\n"); 
            MPI_Abort(MPI_COMM_WORLD, 1); 
        } 
         arr = (int *)malloc(n * sizeof(int));         if (arr == NULL) { 
            fprintf(stderr, "Memory allocation failed.\n"); 
            MPI_Abort(MPI_COMM_WORLD, 1); 
        } 
         printf("Enter the elements:\n");         for (int i = 0; i < n; i++) {             scanf("%d", &arr[i]); 
        } 
    } 
 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);     local_n = n / size; 
     int *local_arr = (int *)malloc(local_n * sizeof(int));     if (local_arr == NULL) { 
        fprintf(stderr, "Memory allocation failed for process %d.\n", rank); 
        MPI_Abort(MPI_COMM_WORLD, 1);     } 
