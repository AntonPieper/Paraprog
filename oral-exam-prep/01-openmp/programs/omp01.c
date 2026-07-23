#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    long n = (argc > 1) ? atol(argv[1]) : 100000000L;
    double *a = malloc(n * sizeof(double));
    for (long i = 0; i < n; i++)
        a[i] = 0.5 + (double)(i % 10);

    double sum = 0.0;
    double t0 = omp_get_wtime();
    #pragma omp parallel for schedule(static) reduction(+:sum)
    for (long i = 0; i < n; i++)
        sum += a[i];
    double t1 = omp_get_wtime();

    printf("sum = %.1f, time = %.3f s\n", sum, t1 - t0);
    free(a);
    return 0;
}
