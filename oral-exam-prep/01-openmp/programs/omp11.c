#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CUTOFF 20

static long fib(int n) {
    if (n < 2)
        return n;
    if (n < CUTOFF)
        return fib(n - 1) + fib(n - 2);
    long x, y;
    #pragma omp task shared(x) firstprivate(n)
    x = fib(n - 1);
    #pragma omp task shared(y) firstprivate(n)
    y = fib(n - 2);
    #pragma omp taskwait
    return x + y;
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 42;
    long result = 0;

    double t0 = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
    result = fib(n);
    double t1 = omp_get_wtime();

    printf("fib(%d) = %ld, time = %.3f s\n", n, result, t1 - t0);
    return 0;
}
