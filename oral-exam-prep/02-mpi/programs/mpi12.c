#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long n = (argc > 1) ? atol(argv[1]) : 1000000L;

    long chunk = n / size;
    long start = rank * chunk + 1;
    long end   = start + chunk;

    long local_sum = 0;
    for (long i = start; i < end; i++)
        local_sum += i;

    long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (rank == 0)
        printf("sum(1..%ld) = %ld (expected: %ld)\n",
               n, global_sum, n * (n + 1) / 2);

    MPI_Finalize();
    return 0;
}
