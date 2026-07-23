#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long num_steps = (argc > 1) ? atol(argv[1]) : 100000000L;
    double step = 1.0 / (double)num_steps;

    double local_sum = 0.0;
    for (long i = rank; i < num_steps; i += size) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (rank == 0)
        printf("pi = %.10f\n", step * global_sum);

    MPI_Finalize();
    return 0;
}
