#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int params[2];
    if (rank == 0) {
        params[0] = 1000;   /* problem size */
        params[1] = 42;     /* seed */
        MPI_Bcast(params, 2, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(params, 2, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    printf("Rank %d: size = %d, seed = %d\n", rank, params[0], params[1]);

    MPI_Finalize();
    return 0;
}
