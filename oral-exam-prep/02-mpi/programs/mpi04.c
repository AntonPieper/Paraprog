#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "This program requires at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int token;
    if (rank == 0) {
        token = 0;
        MPI_Send(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Final token: %d (expected: %d)\n",
               token, (size - 1) * size / 2);
    } else {
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        token += rank;
        MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
