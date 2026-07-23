#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 20000;
    long total = 0;

    double t0 = omp_get_wtime();
    #pragma omp parallel for schedule(static) reduction(+:total)
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            total += (long)((i * j) % 7);
    double t1 = omp_get_wtime();

    printf("total = %ld, time = %.3f s\n", total, t1 - t0);
    return 0;
}
