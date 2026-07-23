#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            fprintf(stderr, "This program requires exactly 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int partner = 1 - rank;
    int mine = rank * 100;
    int theirs = -1;

    MPI_Recv(&theirs, 1, MPI_INT, partner, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Send(&mine, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);

    printf("Rank %d received %d from rank %d\n", rank, theirs, partner);

    MPI_Finalize();
    return 0;
}
