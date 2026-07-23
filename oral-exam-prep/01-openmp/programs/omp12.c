#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 1000000;
    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *c = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++)
        a[i] = (double)i;

    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
        for (int i = 0; i < n; i++)
            b[i] = 2.0 * a[i];

        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++)
            c[i] = b[i] + b[n - 1 - i];
    }

    printf("c[0] = %.1f (expected: %.1f)\n", c[0], 2.0 * (n - 1));
    free(a); free(b); free(c);
    return 0;
}
