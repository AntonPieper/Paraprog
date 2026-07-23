#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    long num_steps = (argc > 1) ? atol(argv[1]) : 10000000L;
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;

    double t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        #pragma omp critical
        sum += 4.0 / (1.0 + x * x);
    }
    double t1 = omp_get_wtime();

    printf("pi = %.10f, time = %.3f s\n", step * sum, t1 - t0);
    return 0;
}
