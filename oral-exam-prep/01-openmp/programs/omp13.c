#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BINS 256

int main(int argc, char **argv) {
    long n = (argc > 1) ? atol(argv[1]) : 100000000L;
    long hist[BINS] = {0};

    double t0 = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (long i = 0; i < n; i++) {
        int b = (int)((i * 2654435761UL + 12345UL) % BINS);
        #pragma omp critical
        hist[b]++;
    }
    double t1 = omp_get_wtime();

    long sum = 0;
    for (int b = 0; b < BINS; b++)
        sum += hist[b];
    printf("sum = %ld (n = %ld), time = %.3f s\n", sum, n, t1 - t0);
    return 0;
}
