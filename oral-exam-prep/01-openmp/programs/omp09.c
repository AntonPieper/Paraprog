#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 1000000;
    double *a = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++)
        a[i] = 1.0;

    #pragma omp parallel for
    for (int i = 1; i < n; i++)
        a[i] = a[i] + a[i - 1];

    printf("a[n-1] = %.1f (expected: %d)\n", a[n - 1], n);
    free(a);
    return 0;
}
