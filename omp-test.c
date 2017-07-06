#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void main() {
    int nThreads = omp_get_num_threads();

    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("hello ");
        printf("world (%d) \n", ID);
    };
}