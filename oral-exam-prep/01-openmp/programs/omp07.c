#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    double *a = malloc((size_t)n * n * sizeof(double));
    double *b = malloc((size_t)n * n * sizeof(double));
    double *c = calloc((size_t)n * n, sizeof(double));
    for (int i = 0; i < n * n; i++) {
        a[i] = (double)(i % 100) / 100.0;
        b[i] = (double)(i % 50) / 50.0;
    }

    double t0 = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double aik = a[i * n + k];
            for (int j = 0; j < n; j++)
                c[i * n + j] += aik * b[k * n + j];
        }
    double t1 = omp_get_wtime();

    double checksum = 0.0;
    for (int i = 0; i < n * n; i++)
        checksum += c[i];
    printf("checksum = %.2f, time = %.3f s\n", checksum, t1 - t0);
    free(a); free(b); free(c);
    return 0;
}
