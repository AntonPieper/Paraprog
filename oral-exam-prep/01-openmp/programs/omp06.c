#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define T 8

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 100000000;
    int *a = malloc(n * sizeof(int));
    srand(42);
    for (int i = 0; i < n; i++)
        a[i] = rand() % 10;

    int private_count[T] = {0};
    int count = 0;

    omp_set_num_threads(T);
    double t0 = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp for
        for (int i = 0; i < n; i++)
            if (a[i] == 3)
                private_count[tid]++;
        #pragma omp atomic
        count += private_count[tid];
    }
    double t1 = omp_get_wtime();

    printf("number of 3s: %d, time = %.3f s\n", count, t1 - t0);
    free(a);
    return 0;
}
