#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 10000000;
    int *a = malloc(n * sizeof(int));
    srand(42);
    for (int i = 0; i < n; i++)
        a[i] = rand() % 10;

    int count = 0;
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        if (a[i] == 3)
            count++;

    printf("number of 3s: %d\n", count);
    free(a);
    return 0;
}
