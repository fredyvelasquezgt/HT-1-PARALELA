//Fredy Velasquez
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int num_threads = (argc > 1) ? atoi(argv[1]) : 10; 

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_num = omp_get_thread_num(); 
        int total_threads = omp_get_num_threads(); 
        printf("Hello from thread %d of %d!\n", thread_num, total_threads); 
    }

    return 0;
}
