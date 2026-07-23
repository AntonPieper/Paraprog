#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    long num_steps = (argc > 1) ? atol(argv[1]) : 100000000L;
    double step = 1.0 / (double)num_steps;
    double x, sum = 0.0;

    #pragma omp parallel for
    for (long i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    printf("pi = %.10f\n", step * sum);
    return 0;
}
